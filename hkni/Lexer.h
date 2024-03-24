#ifndef HKNI_LEXER_H
#define HKNI_LEXER_H

#include <string>
#include <map>
#include <iostream>

#include "Token.h"

using namespace std;

namespace hkni {
    //词法分析模式
    enum LexerMode {
        INPUT_MODE = 1, //代码模式
        FILE_MODE = 2, //文件模式
    };

    const string ErrWrongLexerMode = "错误。错误的词法分析模式。";

    //词法分析器
    class Lexer {
        //##属性
    private:
        string input; //输入
        LexerMode mode; //模式

        FILE *p7file = nullptr; //文件模式打开的文件

        char readBuffer[128] = {0}; //读取缓冲区
        const int readBufferLen = 128; //读取缓冲区的长度
        int nowReadBufferLen = 0; //本次读到缓冲区中的文本长度

        char nextCharBuffer[1] = {0}; // 读取下一个字符的时候用的1个字节的缓冲区

        char nowChar = 0; //当前读取的字符
        int nowCharIndex = 0; //当前读取的字符下标
        int nextCharIndex = 0; //下一个读取的字符下标

        int nowRow = 0; //当前读到的行数
        int nowColumn = 0; //当前读到的列数

        //关键字
        map<string, TokenType> keywordMap = {
                {"null",     NULL_HKNI},
                {"bool",     BOOL_TYPE},
                {"true",     BOOL_VALUE},
                {"false",    BOOL_VALUE},
                {"int",      INT_TYPE},
                {"float",    FLOAT_TYPE},
                {"string",   STRING_TYPE},
                {"var",      VAR_HKNI},
                {"const",    CONST_HKNI},
                {"if",       IF_HKNI},
                {"else",     ELSE_HKNI},
                {"for",      FOR_HKNI},
                {"function", FUNCTION_HKNI},
                {"return",   RETURN_HKNI},
                {"struct",   STRUCT_HKNI},
                {"new",      NEW_HKNI},
        };
        //##方法
    public:
        /**
         * @param input 代码文本、文件路径
         * @param mode
         */
        Lexer(string input, LexerMode mode) {
            this->input = input;
            this->mode = mode;
            if (mode == FILE_MODE) {
                openFile(input);
            }

            readNextChar(); //相当于初始化

            // 读到非0字符，则至少有一行
            if (nowChar != 0) {
                nowRow = 1;
            }
        };

        ~Lexer() {
            if (p7file != nullptr) {
                fclose(p7file);
            }
        };

        string GetModeStr() {
            if (mode == INPUT_MODE) {
                return "代码模式。";
            } else if (mode == FILE_MODE) {
                return "文件模式：" + input + "。";
            } else {
                return ErrWrongLexerMode;
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
                case 0:
                    token.Literal = "";
                    token.TokenType = END;
                    break;
                case '`':
                    token.Literal = readString('`');
                    token.TokenType = STRING_VALUE;
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
                case '(':
                    token.Literal = "(";
                    token.TokenType = LPAREN;
                    break;
                case ')':
                    token.Literal = ")";
                    token.TokenType = RPAREN;
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
                case '[':
                    token.Literal = "[";
                    token.TokenType = LBRACKET;
                    break;
                case '{':
                    token.Literal = "{";
                    token.TokenType = LBRACE;
                    break;
                case ']':
                    token.Literal = "]";
                    token.TokenType = RBRACKET;
                    break;
                case '}':
                    token.Literal = "}";
                    token.TokenType = RBRACE;
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
                case ';':
                    token.Literal = ";";
                    token.TokenType = SEMICOLON;
                    break;
                case ':':
                    token.Literal = ":";
                    token.TokenType = COLON;
                    break;
                case '\'':
                    token.Literal = readString('\'');
                    token.TokenType = STRING_VALUE;
                    break;
                case '"':
                    token.Literal = readString('"');
                    token.TokenType = STRING_VALUE;
                    break;
                case ',':
                    token.Literal = ",";
                    token.TokenType = COMMA;
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
                case '.':
                    token.Literal = ".";
                    token.TokenType = DOT;
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
                default:
                    //如果不是运算符或者字符串，那么就是标识符、关键字、数字
                    if (isAlpha()) {
                        token.Literal = readIdentifier();
                        token.TokenType = matchKeyword(token.Literal);
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
        void openFile(string path) {
            p7file = fopen(path.c_str(), "r");
            if (p7file == nullptr) {
                cout << "错误。文件 " << p7file << " 打开失败。" << endl;
                exit(1);
            }
        };

        //传入词法标记原始值，看看是不是关键字，如果不是关键字，那就是标识符
        TokenType matchKeyword(string literal) {
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
                //代码模式
                if (nextCharIndex >= input.length()) {
                    //读到输入的末尾就结束了
                    nowChar = 0;
                    return;
                }

                nowChar = input[nextCharIndex];

                nowColumn++;
            } else if (mode == FILE_MODE) {
                //文件模式
                if (p7file == nullptr) {
                    nowChar = 0;
                    return;
                }

                if (nextCharIndex == nowReadBufferLen) {
                    //读到缓冲区的末尾，表示缓冲区中的数据已经处理完了
                    //但是这并不表示结束了，文件中有可能还有数据，需要继续读取
                    //要么是第一次调用这个方法，两个值都是 0 的时候进来
                    //要么是一段数据处理完了，需要读取下一段数据的时候进来

                    size_t freadSize = fread(readBuffer, 1, readBufferLen, p7file);
                    nowReadBufferLen = (int) freadSize;

                    if (nowReadBufferLen == 0) {
                        //没读到数据，表示文件已经读取完了
                        //处理一下缓冲区，让 nowChar 拿到 0，结束读取
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

                if (nowChar == 0) {
                    //读到文件末尾，关闭文件，结束读取
                    fclose(p7file);
                    p7file = nullptr;
                    return;
                }
            } else {
                cout << ErrWrongLexerMode << endl;
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
                    //读到缓冲区的末尾，表示缓冲区中的数据已经处理完了
                    //但是这并不表示结束了，文件中有可能还有数据，需要继续读取
                    nextCharBuffer[0] = 0; //重置缓冲区
                    size_t freadSize = fread(nextCharBuffer, 1, 1, p7file);
                    if (freadSize == 0) {
                        return 0;
                    } else {
                        return nextCharBuffer[0];
                    }
                }
                return readBuffer[nextCharIndex];
            } else {
                cout << ErrWrongLexerMode << endl;
                exit(1);
            }
        }


        //字符串，target可以传：单引号（'）、双引号（"）、反引号（`）
        string readString(char target) {
            string str;
            readNextChar();
            while (nowChar != target) {
                if (nowChar == '\\') {
                    readNextChar();
                    if (nowChar == target) { str.push_back(target); }
                    else if (nowChar == '\\') { str.push_back('\\'); }
                    else if (nowChar == '0') { str.push_back('\0'); }
                    else if (nowChar == 't') { str.push_back('\t'); }
                    else if (nowChar == 'r') { str.push_back('\r'); }
                    else if (nowChar == 'n') { str.push_back('\n'); }
                    else { str.push_back(nowChar); }
                } else {
                    str.push_back(nowChar);
                }
                readNextChar();
            }
            return str;
        }

        //a~z A~Z _
        bool isAlpha() {
            return ('a' <= nowChar && nowChar <= 'z') ||
                   ('A' <= nowChar && nowChar <= 'Z') ||
                   nowChar == '_';
        }

        //读取标识符
        string readIdentifier() {
            string str;
            while (isAlphaAndNumber()) {
                str.push_back(nowChar);
                readNextChar();
            }
            return str;
        }

        //a~z A~Z _ 0~9
        bool isAlphaAndNumber() {
            return ('a' <= nowChar && nowChar <= 'z') ||
                   ('A' <= nowChar && nowChar <= 'Z') ||
                   nowChar == '_' ||
                   ('0' <= nowChar && nowChar <= '9');
        }

        //0~9
        bool isNumber() {
            return ('0' <= nowChar && nowChar <= '9');
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

        //读取数字
        string readNumber(bool *isInt, bool *isError) {
            string str;
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
                        str.push_back(nowChar);
                        readNextChar();
                    }
                } else {
                    int num = 0;
                    //2进制、8进制、16进制暂时不支持浮点数
                    readNextChar();
                    if (nowChar == 'b') {
                        //0b开头，2进制；
                        readNextChar();
                        while (isBinNumber()) {
                            num = num * 2 + nowChar - '0';
                            readNextChar();
                        }
                        str = to_string(num);
                    } else if (isNumber()) {
                        //0开头，8进制；
                        while (isOctNumber()) {
                            num = num * 8 + nowChar - '0';
                            readNextChar();
                        }
                        str = to_string(num);
                    } else if (nowChar == 'x') {
                        //0x开头，16进制；
                        readNextChar();
                        while (isHexNumber()) {
                            num = num * 16;
                            if (isNumber()) {
                                num = num + nowChar - '0';
                            } else if ('a' <= nowChar && nowChar <= 'f') {
                                num = num + nowChar - 'a' + 10;
                            } else if ('A' <= nowChar && nowChar <= 'F') {
                                num = num + nowChar - 'A' + 10;
                            }
                            readNextChar();
                        }
                        str = to_string(num);
                    } else if (nowChar == ';' || nowChar == ')'){
                        //就是0的情况，后面要么是分号，要么是右括号
                        str = to_string(num);
                    }else {
                        *isError = true; //错误，识别不出来
                    }
                }
            }

            return str;
        }
    };
}

#endif
