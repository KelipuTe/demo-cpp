#ifndef DEMO_CPP_PARSER_H
#define DEMO_CPP_PARSER_H

#include "Lexer.h"
#include "Token.h"

using namespace std;

//语法分析器
//基于运算符优先级分析
class C5Parser {
    //####属性
private:
    C5Lexer *p7c5l;

    C5Token nowToken;
    C5Token peekToken;

//    map<TOKEN_TYPE,>
    //####方法
public:
    C5Parser(C5Lexer *p7c5l) {
        this->p7c5l = p7c5l;

        // 初始化nowToken和peekToken
        F8GetNextToken();
        F8GetNextToken();
    };

    ~C5Parser() {
        if (p7c5l != nullptr) {
            delete p7c5l;

        }
    };

    void F8GetNextToken() {
        nowToken = peekToken;
        peekToken = p7c5l->F8GetNextToken();
    };
};

#endif //DEMO_CPP_PARSER_H
