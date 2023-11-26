#ifndef DEMO_CPP_LEXER_H
#define DEMO_CPP_LEXER_H

#include <string>
#include <iostream>
#include "Token.h"

using namespace std;

//词法分析器模式
enum LEXER_MODE {
    INPUT_MODE = 1, //代码模式
    FILE_MODE = 2, //文件模式
};

//词法分析器
class C5Lexer {
    //####属性
private:
    string input; //输入
    int mode; //模式

    FILE *inputFile = nullptr; //文件模式打开的文件

    const int readBufferLen = 128; //读取缓冲区的长度
    char readBuffer[128] = {0}; //读取缓冲区
    int nowBufferLen = 0; //本次读到缓冲区中的文本长度
    char nextBuffer[1] = {0}; // peek 的时候用的1个字符的缓冲区

    char readChar = 0; //当前读取的字符
    int readCharIndex = 0; //当前读取的字符下标
    int nextCharIndex = 0; //下一个读取的字符下标

    int nowRow = 0; //当前读到的行数
    int nowColumn = 0; //当前读到的列数

    map<string, TOKEN_TYPE> m3keyword = {
            {"true",   HKNI_TRUE},
            {"false",  HKNI_FALSE},

            {"var",    HKNI_VAR},
            {"const",  HKNI_CONST},

            {"if",     HKNI_IF},
            {"else",   HKNI_ELSE},

            {"while",  HKNI_WHILE},
            {"for",    HKNI_FOR},

            {"func",   HKNI_FUNC},
            {"return", HKNI_RETURN},

            {"class",  HKNI_CLASS},
            {"new",    HKNI_NEW},
    };
    //####方法
public:
    /**
     * @param input 代码文本、文件路径
     * @param mode
     */
    C5Lexer(string input, int mode) {
        this->input = input;
        this->mode = mode;
        if (mode == FILE_MODE) {
            f8OpenFile(input);
        }

        F8ReadNextChar(); //相当于初始化

        // 读到非0字符，则至少有一行
        if (readChar != 0) {
            nowRow = 1;
        }
    };

    ~C5Lexer() {
        if (inputFile != nullptr) {
            fclose(inputFile);
        }
    };

    char F8GetReadChar() {
        return readChar;
    }

    int F8GetNowRow() {
        return nowRow;
    }

    int F8GetNowColumn() {
        return nowColumn;
    }

    void F8ReadNextChar() {
        if (mode == INPUT_MODE) {
            if (nextCharIndex >= input.length()) {
                readChar = 0;
                return;
            }
            readChar = input[nextCharIndex];
            nowColumn++;
        } else if (mode == FILE_MODE) {
            if (inputFile == nullptr) {
                readChar = 0;
                return;
            }

            // 缓冲区中的数据已经处理完了，需要从文件中读取下一段数据
            if (nextCharIndex == nowBufferLen) {
                size_t t4size = fread(readBuffer, 1, readBufferLen, inputFile);
                nowBufferLen = (int) t4size;

                if (nowBufferLen == 0) {
                    readBuffer[0] = 0;
                }

                nextCharIndex = 0;
                readCharIndex = 0;
            }

            readChar = readBuffer[nextCharIndex];

            //读到换行符，行数加一，列数归零
            if (readChar == '\n') {
                nowRow++;
                nowColumn = 0;
            }
            if (readChar != '\n') {
                nowColumn++;
            }

            //读到文件末尾，关闭文件
            if (readChar == 0) {
                fclose(inputFile);
                inputFile = nullptr;
                return;
            }
        } else {
            cout << "词法分析器模式错误。" << endl;
            exit(1);
        }

        readCharIndex = nextCharIndex;
        nextCharIndex++;
    }

    char F8PeekNextChar() {
        if (this->mode == INPUT_MODE) {
            if (this->nextCharIndex >= this->input.length()) {
                return 0;
            }
            return input[nextCharIndex];
        } else if (mode == FILE_MODE) {
            int nextBufferLen;
            // 缓冲区中的数据已经处理完了，需要从文件中读取下一个字符
            if (nextCharIndex == nowBufferLen) {
                nextBuffer[0] = 0; //重置缓冲区
                nextBufferLen = fread(nextBuffer, 1, 1, inputFile);
                if (nextBufferLen == 1) {
                    return nextBuffer[0];
                } else {
                    return 0;
                }
            }
            return readBuffer[nextCharIndex];
        }
    }

    C5Token F8GetNextToken() {
        C5Token token;

        f8SkipWhiteSpace();

        switch (readChar) {
            case '=':
                if (F8PeekNextChar() == '=') {
                    F8ReadNextChar();
                    token.literal = "==";
                    token.token_type = EQ;
                } else {
                    token.literal = "=";
                    token.token_type = ASSIGN;
                }
                break;
            case '+':
                if (F8PeekNextChar() == '=') {
                    F8ReadNextChar();
                    token.literal = "+=";
                    token.token_type = ADD_ASSIGN;
                } else if (F8PeekNextChar() == '+') {
                    F8ReadNextChar();
                    token.literal = "++";
                    token.token_type = INC;
                } else {
                    token.literal = "+";
                    token.token_type = ADD;
                }
            case '-':
                if (F8PeekNextChar() == '=') {
                    F8ReadNextChar();
                    token.literal = "-=";
                    token.token_type = SUB_ASSIGN;
                } else if (F8PeekNextChar() == '-') {
                    F8ReadNextChar();
                    token.literal = "--";
                    token.token_type = DEC;
                } else {
                    token.literal = "-";
                    token.token_type = SUB;
                }
                break;
            case '*':
                if (F8PeekNextChar() == '=') {
                    F8ReadNextChar();
                    token.literal = "*=";
                    token.token_type = MUL_ASSIGN;
                } else {
                    token.literal = "*";
                    token.token_type = MUL;
                }
                break;
            case '/':
                if (F8PeekNextChar() == '=') {
                    F8ReadNextChar();
                    token.literal = "/=";
                    token.token_type = DIV_ASSIGN;
                } else if (F8PeekNextChar() == '/') {
                    //单行注释
                    F8ReadNextChar();

                    string t4str = "//";
                    while (readChar != 0 && readChar != '\n') {
                        F8ReadNextChar();
                        t4str.push_back(readChar);
                    }

                    token.literal = t4str;
                    token.token_type = COMMENT;
                } else if (F8PeekNextChar() == '*') {
                    //多行注释
                    F8ReadNextChar();

                    string t4str = "/*";
                    while (readChar != 0) {
                        F8ReadNextChar();
                        t4str.push_back(readChar);
                        if (readChar == '*' && F8PeekNextChar() == '/') {
                            F8ReadNextChar();
                            t4str.push_back(readChar);
                            break;
                        }
                    }

                    token.literal = t4str;
                    token.token_type = COMMENT;
                } else {
                    token.literal = "/";
                    token.token_type = DIV;
                }
                break;
            case '%':
                if (F8PeekNextChar() == '=') {
                    F8ReadNextChar();
                    token.literal = "%=";
                    token.token_type = MOD_ASSIGN;
                } else {
                    token.literal = "%";
                    token.token_type = MOD;
                }
                break;
            case '>':
                if (F8PeekNextChar() == '=') {
                    F8ReadNextChar();
                    token.literal = ">=";
                    token.token_type = GTE;
                } else {
                    token.literal = ">";
                    token.token_type = GT;
                }
                break;
            case '<':
                if (F8PeekNextChar() == '=') {
                    F8ReadNextChar();
                    token.literal = "<=";
                    token.token_type = LTE;
                } else {
                    token.literal = "<";
                    token.token_type = LT;
                }
                break;
            case '&':
                if (F8PeekNextChar() == '&') {
                    F8ReadNextChar();
                    token.literal = "&&";
                    token.token_type = AND;
                } else {
                    token.literal = "&";
                    token.token_type = BIT_AND;
                }
                break;
            case '|':
                if (F8PeekNextChar() == '|') {
                    F8ReadNextChar();
                    token.literal = "||";
                    token.token_type = OR;
                } else {
                    token.literal = "|";
                    token.token_type = BIT_OR;
                }
                break;
            case '!':
                if (F8PeekNextChar() == '=') {
                    F8ReadNextChar();
                    token.literal = "!=";
                    token.token_type = NEQ;
                } else {
                    token.literal = "!";
                    token.token_type = NOT;
                }
                break;
            case '\'':
                token.literal = f8ReadString('\'');
                token.token_type = HKNI_STRING;
                break;
            case '"':
                token.literal = f8ReadString('"');
                token.token_type = HKNI_STRING;
                break;
            case '`':
                token.literal = f8ReadString('`');
                token.token_type = HKNI_STRING;
                break;
            case ';':
                token.literal = ";";
                token.token_type = SEMICOLON;
                break;
            case ':':
                token.literal = ":";
                token.token_type = COLON;
                break;
            case ',':
                token.literal = ",";
                token.token_type = COMMA;
                break;
            case '.':
                token.literal = ".";
                token.token_type = DOT;
                break;
            case '(':
                token.literal = "(";
                token.token_type = LPAREN;
                break;
            case ')':
                token.literal = ")";
                token.token_type = RPAREN;
                break;
            case '{':
                token.literal = "{";
                token.token_type = LBRACE;
                break;
            case '}':
                token.literal = "}";
                token.token_type = RBRACE;
                break;
            case '[':
                token.literal = "[";
                token.token_type = LBRACKET;
                break;
            case ']':
                token.literal = "]";
                token.token_type = RBRACKET;
                break;
            case 0:
                token.literal = "";
                token.token_type = END;
                break;
            default:
                //如果不是符号或者字符串，那么就是标识符、关键字、数字
                if (f8IsAlpha()) {
                    token.literal = f8ReadIdentifier();
                    token.token_type = f8MatchKeyword(token.literal);
                    return token;
                } else if (f8IsNumber()) {
                    bool isInt = true;
                    bool isError = false;
                    string t4str = f8ReadNumber(&isInt, &isError);
                    if (isError) {
                        cout << "数字格式错误。" << endl;
                        exit(1);
                    }
                    token.literal = t4str;
                    token.token_type = isInt ? HKNI_INT : HKNI_FLOAT;
                    return token;
                } else {
                    cout << "未知字符。" << endl;
                    exit(1);
                }
        }

        F8ReadNextChar();

        return token;
    }

private:
    void f8OpenFile(string path) {
        inputFile = fopen(path.c_str(), "r");
        if (inputFile == nullptr) {
            cout << "文件 " << inputFile << " 打开失败。" << endl;
            exit(1);
        }
    };

    //匹配关键字
    TOKEN_TYPE f8MatchKeyword(string literal) {
        auto token_type = m3keyword.find(literal);
        if (token_type != m3keyword.end()) {
            return token_type->second; //关键字
        }
        return IDENTIFIER; //标识符
    }

    //跳过空白字符（空格、制表符、回车符、换行符）
    void f8SkipWhiteSpace() {
        while (readChar == ' ' ||
               readChar == '\t' ||
               readChar == '\r' ||
               readChar == '\n') {
            F8ReadNextChar();
        }
    }

    //a~z A~Z _
    bool f8IsAlpha() {
        return ('a' <= readChar && readChar <= 'z') ||
               ('A' <= readChar && readChar <= 'Z') ||
               readChar == '_';
    }

    //0~9
    bool f8IsNumber() {
        return ('0' <= readChar && readChar <= '9');
    }

    //a~z A~Z _ 0~9
    bool f8IsAlphaAndNumber() {
        return ('a' <= readChar && readChar <= 'z') ||
               ('A' <= readChar && readChar <= 'Z') ||
               readChar == '_' ||
               ('0' <= readChar && readChar <= '9');
    }

    //16进制数字
    bool f8IsHexNumber() {
        return ('0' <= readChar && readChar <= '9') ||
               ('a' <= readChar && readChar <= 'f') ||
               ('A' <= readChar && readChar <= 'F');
    }

    //8进制数字
    bool f8IsOctNumber() {
        return ('0' <= readChar && readChar <= '7');
    }

    //2进制数字
    bool f8IsBinNumber() {
        return ('0' <= readChar && readChar <= '1');
    }

    //读取标识符
    string f8ReadIdentifier() {
        string t4str;
        while (f8IsAlphaAndNumber()) {
            t4str.push_back(readChar);
            F8ReadNextChar();
        }
        return t4str;
    }

    //读取数字
    string f8ReadNumber(bool *isInt, bool *isError) {
        string t4str;

        if (f8IsNumber()) {
            if (readChar != '0') {
                //不是0开头，10进制
                while (f8IsNumber() || readChar == '.') {
                    if (readChar == '.') {
                        if (*isInt) {
                            *isInt = false;
                        } else {
                            //小数点出现两次
                            *isError = true;
                        }
                    }
                    t4str.push_back(readChar);
                    F8ReadNextChar();
                }
            } else {
                int t4num = 0;
                //这三种类型暂时不支持浮点数
                F8ReadNextChar();
                if (readChar == 'x') {
                    //0x开头，16进制；
                    F8ReadNextChar();
                    while (f8IsHexNumber()) {
                        t4num = t4num * 16;
                        if (f8IsNumber()) {
                            t4num = t4num + readChar - '0';
                        } else if ('a' <= readChar && readChar <= 'f') {
                            t4num = t4num + readChar - 'a' + 10;
                        } else if ('A' <= readChar && readChar <= 'F') {
                            t4num = t4num + readChar - 'A' + 10;
                        }
                        F8ReadNextChar();
                    }
                    t4str = to_string(t4num);
                } else if (f8IsNumber()) {
                    //0开头，8进制；
                    while (f8IsOctNumber()) {
                        t4num = t4num * 8 + readChar - '0';
                        F8ReadNextChar();
                    }
                    t4str = to_string(t4num);
                } else if (readChar == 'b') {
                    //0b开头，2进制；
                    F8ReadNextChar();
                    while (f8IsBinNumber()) {
                        t4num = t4num * 2 + readChar - '0';
                        F8ReadNextChar();
                    }
                    t4str = to_string(t4num);
                } else {
                    //小数点出现两次
                    *isError = true;
                }
            }
        }

        return t4str;
    }

    //字符串，target可以传：单引号（'）、双引号（"）、反引号（`）
    string f8ReadString(char target) {
        string t4str;
        F8ReadNextChar();
        while (readChar != target) {
            if (readChar == '\\') {
                F8ReadNextChar();
                if (readChar == target) { t4str.push_back(target); }
                else if (readChar == '\\') { t4str.push_back('\\'); }
                else if (readChar == '0') { t4str.push_back('\0'); }
                else if (readChar == 't') { t4str.push_back('\t'); }
                else if (readChar == 'r') { t4str.push_back('\r'); }
                else if (readChar == 'n') { t4str.push_back('\n'); }
                else { t4str.push_back(readChar); }
            } else {
                t4str.push_back(readChar);
            }
            F8ReadNextChar();
        }
        return t4str;
    }
};

#endif //DEMO_CPP_LEXER_H
