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

const string ERR_LEXER_MODE = "错误。词法分析模式错误。";

//词法分析器
class Lexer {
    //####属性
private:
    string input; //输入
    LEXER_MODE mode; //模式

    FILE *p7InputFile = nullptr; //文件模式打开的文件

    char readBuffer[128] = {0}; //读取缓冲区
    const int readBufferLen = 128; //读取缓冲区的长度
    int nowReadBufferLen = 0; //本次读到缓冲区中的文本长度

    char nextCharBuffer[1] = {0}; // 读取下一个字符的时候用的1个字节的缓冲区

    char nowChar = 0; //当前读取的字符
    int nowCharIndex = 0; //当前读取的字符下标
    int nextCharIndex = 0; //下一个读取的字符下标

    int nowRow = 0; //当前读到的行数
    int nowColumn = 0; //当前读到的列数

    map<string, TOKEN_TYPE> keywordMap = {
            {"null",   NULL_HKNI},

            {"bool",   BOOL_TYPE},
            {"true",   TRUE_VALUE},
            {"false",  FALSE_VALUE},

            {"int",    INT_TYPE},
            {"float",  FLOAT_TYPE},
            {"string", STRING_TYPE},

            {"const",  CONST_HKNI},

            {"if",     IF_HKNI},
            {"else",   ELSE_HKNI},

            {"for",    FOR_HKNI},

            {"func",   FUNC_HKNI},
            {"return", RETURN_HKNI},

            {"class",  CLASS_HKNI},
            {"new",    NEW_HKNI},
    }; //关键字
    //####方法
public:
    /**
     * @param input 代码文本、文件路径
     * @param mode
     */
    Lexer(string input, LEXER_MODE mode) {
        this->input = input;
        this->mode = mode;
        if (mode == FILE_MODE) {
            f8OpenFile(input);
        }

        readNextChar(); //相当于初始化

        // 读到非0字符，则至少有一行
        if (nowChar != 0) {
            nowRow = 1;
        }
    };

    ~Lexer() {
        if (p7InputFile != nullptr) {
            fclose(p7InputFile);
        }
    };

    string GetModeStr() {
        if (mode == INPUT_MODE) {
            return "代码模式。";
        } else if (mode == FILE_MODE) {
            return "文件模式：" + input + "。";
        } else {
            return ERR_LEXER_MODE;
        }
    }

    int GetNowRow() {
        return nowRow;
    }

    int GetNowColumn() {
        return nowColumn;
    }

    Token GetNextToken() {
        Token token;

        skipWhiteSpace();

        switch (nowChar) {
            case '+':
                if (peekNextChar() == '=') {
                    readNextChar();
                    token.Literal = "+=";
                    token.TokenType = ADD_ASSIGN;
                } else if (peekNextChar() == '+') {
                    readNextChar();
                    token.Literal = "++";
                    token.TokenType = INC;
                } else {
                    token.Literal = "+";
                    token.TokenType = ADD;
                }
                break;
            case '-':
                if (peekNextChar() == '=') {
                    readNextChar();
                    token.Literal = "-=";
                    token.TokenType = SUB_ASSIGN;
                } else if (peekNextChar() == '-') {
                    readNextChar();
                    token.Literal = "--";
                    token.TokenType = DEC;
                } else {
                    token.Literal = "-";
                    token.TokenType = SUB;
                }
                break;
            case '*':
                if (peekNextChar() == '=') {
                    readNextChar();
                    token.Literal = "*=";
                    token.TokenType = MUL_ASSIGN;
                } else {
                    token.Literal = "*";
                    token.TokenType = MUL;
                }
                break;
            case '/':
                if (peekNextChar() == '=') {
                    readNextChar();
                    token.Literal = "/=";
                    token.TokenType = DIV_ASSIGN;
                } else if (peekNextChar() == '/') {
                    //单行注释
                    readNextChar();

                    string t4str = "//";
                    while (nowChar != 0 && nowChar != '\n') {
                        readNextChar();
                        t4str.push_back(nowChar);
                    }

                    token.Literal = t4str;
                    token.TokenType = COMMENT;
                } else if (peekNextChar() == '*') {
                    //多行注释
                    readNextChar();

                    string t4str = "/*";
                    while (nowChar != 0) {
                        readNextChar();
                        t4str.push_back(nowChar);
                        if (nowChar == '*' && peekNextChar() == '/') {
                            readNextChar();
                            t4str.push_back(nowChar);
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
                if (peekNextChar() == '=') {
                    readNextChar();
                    token.Literal = "%=";
                    token.TokenType = MOD_ASSIGN;
                } else {
                    token.Literal = "%";
                    token.TokenType = MOD;
                }
                break;
            case '=':
                if (peekNextChar() == '=') {
                    readNextChar();
                    token.Literal = "==";
                    token.TokenType = EQ;
                } else {
                    token.Literal = "=";
                    token.TokenType = ASSIGN;
                }
                break;
            case '>':
                if (peekNextChar() == '=') {
                    readNextChar();
                    token.Literal = ">=";
                    token.TokenType = GTE;
                } else {
                    token.Literal = ">";
                    token.TokenType = GT;
                }
                break;
            case '<':
                if (peekNextChar() == '=') {
                    readNextChar();
                    token.Literal = "<=";
                    token.TokenType = LTE;
                } else {
                    token.Literal = "<";
                    token.TokenType = LT;
                }
                break;
            case '&':
                if (peekNextChar() == '&') {
                    readNextChar();
                    token.Literal = "&&";
                    token.TokenType = AND;
                } else {
                    token.Literal = "&";
                    token.TokenType = BIT_AND;
                }
                break;
            case '|':
                if (peekNextChar() == '|') {
                    readNextChar();
                    token.Literal = "||";
                    token.TokenType = OR;
                } else {
                    token.Literal = "|";
                    token.TokenType = BIT_OR;
                }
                break;
            case '!':
                if (peekNextChar() == '=') {
                    readNextChar();
                    token.Literal = "!=";
                    token.TokenType = NEQ;
                } else {
                    token.Literal = "!";
                    token.TokenType = NOT;
                }
                break;
            case '\'':
                token.Literal = readString('\'');
                token.TokenType = STRING_VALUE;
                break;
            case '"':
                token.Literal = readString('"');
                token.TokenType = STRING_VALUE;
                break;
            case '`':
                token.Literal = readString('`');
                token.TokenType = STRING_VALUE;
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
            case '[':
                token.Literal = "[";
                token.TokenType = LBRACKET;
                break;
            case ']':
                token.Literal = "]";
                token.TokenType = RBRACKET;
                break;
            case '{':
                token.Literal = "{";
                token.TokenType = LBRACE;
                break;
            case '}':
                token.Literal = "}";
                token.TokenType = RBRACE;
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
                        cout << "错误。数字格式错误。" << endl;
                        exit(1);
                    }
                    token.Literal = t4str;
                    token.TokenType = isInt ? INT_VALUE : FLOAT_VALUE;
                    return token;
                } else {
                    cout << "错误。词法标记异常。" << endl;
                    exit(1);
                }
        }

        readNextChar();

        return token;
    }

private:
    void f8OpenFile(string path) {
        p7InputFile = fopen(path.c_str(), "r");
        if (p7InputFile == nullptr) {
            cout << "错误。文件 " << p7InputFile << " 打开失败。" << endl;
            exit(1);
        }
    };

    //传入词法标记原始值，看看是不是关键字，如果不是关键字，那就是标识符
    TOKEN_TYPE f8MatchKeyword(string literal) {
        auto tokenType = keywordMap.find(literal);
        if (tokenType != keywordMap.end()) {
            return tokenType->second; //关键字
        }
        return IDENTIFIER; //标识符
    }

    //跳过空白字符，空格、制表符、回车符、换行符
    void skipWhiteSpace() {
        while (nowChar == ' ' || nowChar == '\t' || nowChar == '\r' || nowChar == '\n') {
            readNextChar();
        }
    }

    //读取下一个字符，移动当前读取到的下标位置
    void readNextChar() {
        if (mode == INPUT_MODE) {
            if (nextCharIndex >= input.length()) {
                nowChar = 0;
                return;
            }

            nowChar = input[nextCharIndex];

            nowColumn++;
        } else if (mode == FILE_MODE) {
            if (p7InputFile == nullptr) {
                nowChar = 0;
                return;
            }

            if (nextCharIndex == nowReadBufferLen) {
                // 缓冲区中的数据已经处理完了，需要从文件中读取下一段数据

                size_t freadSize = fread(readBuffer, 1, readBufferLen, p7InputFile);
                nowReadBufferLen = (int) freadSize;

                if (nowReadBufferLen == 0) {
                    readBuffer[0] = 0;
                }

                nextCharIndex = 0;
                nowCharIndex = 0;
            }

            nowChar = readBuffer[nextCharIndex];

            if (nowChar == '\n') {
                //读到换行符，行数加一，列数归零
                nowRow++;
                nowColumn = 0;
            } else {
                //不是换行符，行数不变，列数加一
                nowColumn++;
            }

            //读到文件末尾，关闭文件
            if (nowChar == 0) {
                fclose(p7InputFile);
                p7InputFile = nullptr;
                return;
            }
        } else {
            cout << ERR_LEXER_MODE << endl;
            exit(1);
        }

        nowCharIndex = nextCharIndex;
        nextCharIndex++;
    }

    //读取下一个字符，但是不移动当前读取到的下标位置
    char peekNextChar() {
        if (mode == INPUT_MODE) {
            if (nextCharIndex >= input.length()) {
                return 0;
            }
            return input[nextCharIndex];
        } else if (mode == FILE_MODE) {

            if (nextCharIndex == nowReadBufferLen) {
                // 缓冲区中的数据已经处理完了，需要从文件中读取下一个字符

                nextCharBuffer[0] = 0; //重置缓冲区
                size_t freadSize = fread(nextCharBuffer, 1, 1, p7InputFile);
                if (freadSize == 1) {
                    return nextCharBuffer[0];
                } else {
                    return 0;
                }
            }
            return readBuffer[nextCharIndex];
        } else {
            cout << ERR_LEXER_MODE << endl;
            exit(1);
        }
    }

    //a~z A~Z _
    bool isAlpha() {
        return ('a' <= nowChar && nowChar <= 'z') ||
               ('A' <= nowChar && nowChar <= 'Z') ||
               nowChar == '_';
    }

    //0~9
    bool isNumber() {
        return ('0' <= nowChar && nowChar <= '9');
    }

    //a~z A~Z _ 0~9
    bool isAlphaAndNumber() {
        return ('a' <= nowChar && nowChar <= 'z') ||
               ('A' <= nowChar && nowChar <= 'Z') ||
               nowChar == '_' ||
               ('0' <= nowChar && nowChar <= '9');
    }

    //2进制数字
    bool isBinNumber() {
        return ('0' <= nowChar && nowChar <= '1');
    }

    //8进制数字
    bool isOctNumber() {
        return ('0' <= nowChar && nowChar <= '7');
    }

    //16进制数字
    bool isHexNumber() {
        return ('0' <= nowChar && nowChar <= '9') ||
               ('a' <= nowChar && nowChar <= 'f') ||
               ('A' <= nowChar && nowChar <= 'F');
    }

    //读取标识符
    string readIdentifier() {
        string t4str;
        while (isAlphaAndNumber()) {
            t4str.push_back(nowChar);
            readNextChar();
        }
        return t4str;
    }

    //读取数字
    string readNumber(bool *isInt, bool *isError) {
        string t4str;

        if (isNumber()) {
            if (nowChar != '0') {
                //不是0开头，10进制
                while (isNumber() || nowChar == '.') {
                    if (nowChar == '.') {
                        if (*isInt) {
                            *isInt = false;
                        } else {
                            *isError = true; //错误，小数点出现两次
                        }
                    }
                    t4str.push_back(nowChar);
                    readNextChar();
                }
            } else {
                int t4num = 0;
                //2进制、8进制、16进制暂时不支持浮点数
                readNextChar();
                if (nowChar == 'b') {
                    //0b开头，2进制；
                    readNextChar();
                    while (isBinNumber()) {
                        t4num = t4num * 2 + nowChar - '0';
                        readNextChar();
                    }
                    t4str = to_string(t4num);
                } else if (isNumber()) {
                    //0开头，8进制；
                    while (isOctNumber()) {
                        t4num = t4num * 8 + nowChar - '0';
                        readNextChar();
                    }
                    t4str = to_string(t4num);
                } else if (nowChar == 'x') {
                    //0x开头，16进制；
                    readNextChar();
                    while (isHexNumber()) {
                        t4num = t4num * 16;
                        if (isNumber()) {
                            t4num = t4num + nowChar - '0';
                        } else if ('a' <= nowChar && nowChar <= 'f') {
                            t4num = t4num + nowChar - 'a' + 10;
                        } else if ('A' <= nowChar && nowChar <= 'F') {
                            t4num = t4num + nowChar - 'A' + 10;
                        }
                        readNextChar();
                    }
                    t4str = to_string(t4num);
                } else {
                    *isError = true; //错误，识别不出来
                }
            }
        }

        return t4str;
    }

    //字符串，target可以传：单引号（'）、双引号（"）、反引号（`）
    string readString(char target) {
        string t4str;
        readNextChar();
        while (nowChar != target) {
            if (nowChar == '\\') {
                readNextChar();
                if (nowChar == target) { t4str.push_back(target); }
                else if (nowChar == '\\') { t4str.push_back('\\'); }
                else if (nowChar == '0') { t4str.push_back('\0'); }
                else if (nowChar == 't') { t4str.push_back('\t'); }
                else if (nowChar == 'r') { t4str.push_back('\r'); }
                else if (nowChar == 'n') { t4str.push_back('\n'); }
                else { t4str.push_back(nowChar); }
            } else {
                t4str.push_back(nowChar);
            }
            readNextChar();
        }
        return t4str;
    }
};

#endif //HKNI_LEXER_H
