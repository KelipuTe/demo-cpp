#ifndef HKNI_LEXER_H
#define HKNI_LEXER_H

#include <string>
#include <map>
#include <iostream>
#include "Token.h"

using namespace std;

//词法分析模式
enum LEXER_MODE {
    INPUT_MODE = 1, //代码模式
    FILE_MODE = 2, //文件模式
};

//词法分析器
class Lexer {
    //####属性
private:
    string input; //输入
    int mode; //模式

    FILE *p7InputFile = nullptr; //文件模式打开的文件

    const int readBufferLen = 128; //读取缓冲区的长度
    char readBuffer[128] = {0}; //读取缓冲区
    int nowBufferLen = 0; //本次读到缓冲区中的文本长度
    char nextBuffer[1] = {0}; // peek 的时候用的1个字符的缓冲区

    char readChar = 0; //当前读取的字符
    int readCharIndex = 0; //当前读取的字符下标
    int nextCharIndex = 0; //下一个读取的字符下标

    int nowRow = 0; //当前读到的行数
    int nowColumn = 0; //当前读到的列数

    map<string, TOKEN_TYPE> keywordMap = {
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
    }; //关键字
    //####方法
public:
    /**
     * @param input 代码文本、文件路径
     * @param mode
     */
    Lexer(string input, int mode) {
        this->input = input;
        this->mode = mode;
        if (mode == FILE_MODE) {
            f8OpenFile(input);
        }

        ReadNextChar(); //相当于初始化

        // 读到非0字符，则至少有一行
        if (readChar != 0) {
            nowRow = 1;
        }
    };

    ~Lexer() {
        if (p7InputFile != nullptr) {
            fclose(p7InputFile);
        }
    };

    string GetModeStr() {
        if (this->mode == INPUT_MODE) {
            return "代码模式。";
        } else if (this->mode == FILE_MODE) {
            return "文件模式：" + this->input + "。";
        } else {
            return "词法分析模式错误。";
        }
    }

    char GetReadChar() {
        return readChar;
    }

    int GetNowRow() {
        return nowRow;
    }

    int GetNowColumn() {
        return nowColumn;
    }

    void ReadNextChar() {
        if (mode == INPUT_MODE) {
            if (nextCharIndex >= input.length()) {
                readChar = 0;
                return;
            }
            readChar = input[nextCharIndex];
            nowColumn++;
        } else if (mode == FILE_MODE) {
            if (p7InputFile == nullptr) {
                readChar = 0;
                return;
            }

            // 缓冲区中的数据已经处理完了，需要从文件中读取下一段数据
            if (nextCharIndex == nowBufferLen) {
                size_t t4size = fread(readBuffer, 1, readBufferLen, p7InputFile);
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
                fclose(p7InputFile);
                p7InputFile = nullptr;
                return;
            }
        } else {
            cout << "词法分析器模式错误。" << endl;
            exit(1);
        }

        readCharIndex = nextCharIndex;
        nextCharIndex++;
    }

    char PeekNextChar() {
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
                nextBufferLen = fread(nextBuffer, 1, 1, p7InputFile);
                if (nextBufferLen == 1) {
                    return nextBuffer[0];
                } else {
                    return 0;
                }
            }
            return readBuffer[nextCharIndex];
        } else {
            cout << "词法分析器模式错误。" << endl;
            exit(1);
        }
    }

    Token GetNextToken() {
        Token token;

        skipWhiteSpace();

        switch (readChar) {
            case '=':
                if (PeekNextChar() == '=') {
                    ReadNextChar();
                    token.Literal = "==";
                    token.TokenType = EQ;
                } else {
                    token.Literal = "=";
                    token.TokenType = ASSIGN;
                }
                break;
            case '+':
                if (PeekNextChar() == '=') {
                    ReadNextChar();
                    token.Literal = "+=";
                    token.TokenType = ADD_ASSIGN;
                } else if (PeekNextChar() == '+') {
                    ReadNextChar();
                    token.Literal = "++";
                    token.TokenType = INC;
                } else {
                    token.Literal = "+";
                    token.TokenType = ADD;
                }
                break;
            case '-':
                if (PeekNextChar() == '=') {
                    ReadNextChar();
                    token.Literal = "-=";
                    token.TokenType = SUB_ASSIGN;
                } else if (PeekNextChar() == '-') {
                    ReadNextChar();
                    token.Literal = "--";
                    token.TokenType = DEC;
                } else {
                    token.Literal = "-";
                    token.TokenType = SUB;
                }
                break;
            case '*':
                if (PeekNextChar() == '=') {
                    ReadNextChar();
                    token.Literal = "*=";
                    token.TokenType = MUL_ASSIGN;
                } else {
                    token.Literal = "*";
                    token.TokenType = MUL;
                }
                break;
            case '/':
                if (PeekNextChar() == '=') {
                    ReadNextChar();
                    token.Literal = "/=";
                    token.TokenType = DIV_ASSIGN;
                } else if (PeekNextChar() == '/') {
                    //单行注释
                    ReadNextChar();

                    string t4str = "//";
                    while (readChar != 0 && readChar != '\n') {
                        ReadNextChar();
                        t4str.push_back(readChar);
                    }

                    token.Literal = t4str;
                    token.TokenType = COMMENT;
                } else if (PeekNextChar() == '*') {
                    //多行注释
                    ReadNextChar();

                    string t4str = "/*";
                    while (readChar != 0) {
                        ReadNextChar();
                        t4str.push_back(readChar);
                        if (readChar == '*' && PeekNextChar() == '/') {
                            ReadNextChar();
                            t4str.push_back(readChar);
                            break;
                        }
                    }

                    token.Literal = t4str;
                    token.TokenType = COMMENT;
                } else {
                    token.Literal = "/";
                    token.TokenType = DIV;
                }
                break;
            case '%':
                if (PeekNextChar() == '=') {
                    ReadNextChar();
                    token.Literal = "%=";
                    token.TokenType = MOD_ASSIGN;
                } else {
                    token.Literal = "%";
                    token.TokenType = MOD;
                }
                break;
            case '>':
                if (PeekNextChar() == '=') {
                    ReadNextChar();
                    token.Literal = ">=";
                    token.TokenType = GTE;
                } else {
                    token.Literal = ">";
                    token.TokenType = GT;
                }
                break;
            case '<':
                if (PeekNextChar() == '=') {
                    ReadNextChar();
                    token.Literal = "<=";
                    token.TokenType = LTE;
                } else {
                    token.Literal = "<";
                    token.TokenType = LT;
                }
                break;
            case '&':
                if (PeekNextChar() == '&') {
                    ReadNextChar();
                    token.Literal = "&&";
                    token.TokenType = AND;
                } else {
                    token.Literal = "&";
                    token.TokenType = BIT_AND;
                }
                break;
            case '|':
                if (PeekNextChar() == '|') {
                    ReadNextChar();
                    token.Literal = "||";
                    token.TokenType = OR;
                } else {
                    token.Literal = "|";
                    token.TokenType = BIT_OR;
                }
                break;
            case '!':
                if (PeekNextChar() == '=') {
                    ReadNextChar();
                    token.Literal = "!=";
                    token.TokenType = NEQ;
                } else {
                    token.Literal = "!";
                    token.TokenType = NOT;
                }
                break;
            case '\'':
                token.Literal = readString('\'');
                token.TokenType = HKNI_STRING;
                break;
            case '"':
                token.Literal = readString('"');
                token.TokenType = HKNI_STRING;
                break;
            case '`':
                token.Literal = readString('`');
                token.TokenType = HKNI_STRING;
                break;
            case ';':
                token.Literal = ";";
                token.TokenType = SEMICOLON;
                break;
            case ':':
                token.Literal = ":";
                token.TokenType = COLON;
                break;
            case ',':
                token.Literal = ",";
                token.TokenType = COMMA;
                break;
            case '.':
                token.Literal = ".";
                token.TokenType = DOT;
                break;
            case '(':
                token.Literal = "(";
                token.TokenType = LPAREN;
                break;
            case ')':
                token.Literal = ")";
                token.TokenType = RPAREN;
                break;
            case '{':
                token.Literal = "{";
                token.TokenType = LBRACE;
                break;
            case '}':
                token.Literal = "}";
                token.TokenType = RBRACE;
                break;
            case '[':
                token.Literal = "[";
                token.TokenType = LBRACKET;
                break;
            case ']':
                token.Literal = "]";
                token.TokenType = RBRACKET;
                break;
            case 0:
                token.Literal = "";
                token.TokenType = END;
                break;
            default:
                //如果不是符号或者字符串，那么就是标识符、关键字、数字
                if (isAlpha()) {
                    token.Literal = readIdentifier();
                    token.TokenType = f8MatchKeyword(token.Literal);
                    return token;
                } else if (isNumber()) {
                    bool isInt = true;
                    bool isError = false;
                    string t4str = readNumber(&isInt, &isError);
                    if (isError) {
                        cout << "数字格式错误。" << endl;
                        exit(1);
                    }
                    token.Literal = t4str;
                    token.TokenType = isInt ? HKNI_INT : HKNI_FLOAT;
                    return token;
                } else {
                    cout << "未知字符。" << endl;
                    exit(1);
                }
        }

        ReadNextChar();

        return token;
    }

private:
    void f8OpenFile(string path) {
        p7InputFile = fopen(path.c_str(), "r");
        if (p7InputFile == nullptr) {
            cout << "文件 " << p7InputFile << " 打开失败。" << endl;
            exit(1);
        }
    };

    //匹配关键字
    TOKEN_TYPE f8MatchKeyword(string literal) {
        auto token_type = keywordMap.find(literal);
        if (token_type != keywordMap.end()) {
            return token_type->second; //关键字
        }
        return IDENTIFIER; //标识符
    }

    //跳过空白字符（空格、制表符、回车符、换行符）
    void skipWhiteSpace() {
        while (readChar == ' ' ||
               readChar == '\t' ||
               readChar == '\r' ||
               readChar == '\n') {
            ReadNextChar();
        }
    }

    //a~z A~Z _
    bool isAlpha() {
        return ('a' <= readChar && readChar <= 'z') ||
               ('A' <= readChar && readChar <= 'Z') ||
               readChar == '_';
    }

    //0~9
    bool isNumber() {
        return ('0' <= readChar && readChar <= '9');
    }

    //a~z A~Z _ 0~9
    bool isAlphaAndNumber() {
        return ('a' <= readChar && readChar <= 'z') ||
               ('A' <= readChar && readChar <= 'Z') ||
               readChar == '_' ||
               ('0' <= readChar && readChar <= '9');
    }

    //16进制数字
    bool isHexNumber() {
        return ('0' <= readChar && readChar <= '9') ||
               ('a' <= readChar && readChar <= 'f') ||
               ('A' <= readChar && readChar <= 'F');
    }

    //8进制数字
    bool isOctNumber() {
        return ('0' <= readChar && readChar <= '7');
    }

    //2进制数字
    bool isBinNumber() {
        return ('0' <= readChar && readChar <= '1');
    }

    //读取标识符
    string readIdentifier() {
        string t4str;
        while (isAlphaAndNumber()) {
            t4str.push_back(readChar);
            ReadNextChar();
        }
        return t4str;
    }

    //读取数字
    string readNumber(bool *isInt, bool *isError) {
        string t4str;

        if (isNumber()) {
            if (readChar != '0') {
                //不是0开头，10进制
                while (isNumber() || readChar == '.') {
                    if (readChar == '.') {
                        if (*isInt) {
                            *isInt = false;
                        } else {
                            //小数点出现两次
                            *isError = true;
                        }
                    }
                    t4str.push_back(readChar);
                    ReadNextChar();
                }
            } else {
                int t4num = 0;
                //这三种类型暂时不支持浮点数
                ReadNextChar();
                if (readChar == 'x') {
                    //0x开头，16进制；
                    ReadNextChar();
                    while (isHexNumber()) {
                        t4num = t4num * 16;
                        if (isNumber()) {
                            t4num = t4num + readChar - '0';
                        } else if ('a' <= readChar && readChar <= 'f') {
                            t4num = t4num + readChar - 'a' + 10;
                        } else if ('A' <= readChar && readChar <= 'F') {
                            t4num = t4num + readChar - 'A' + 10;
                        }
                        ReadNextChar();
                    }
                    t4str = to_string(t4num);
                } else if (isNumber()) {
                    //0开头，8进制；
                    while (isOctNumber()) {
                        t4num = t4num * 8 + readChar - '0';
                        ReadNextChar();
                    }
                    t4str = to_string(t4num);
                } else if (readChar == 'b') {
                    //0b开头，2进制；
                    ReadNextChar();
                    while (isBinNumber()) {
                        t4num = t4num * 2 + readChar - '0';
                        ReadNextChar();
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
    string readString(char target) {
        string t4str;
        ReadNextChar();
        while (readChar != target) {
            if (readChar == '\\') {
                ReadNextChar();
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
            ReadNextChar();
        }
        return t4str;
    }
};

#endif //HKNI_LEXER_H
