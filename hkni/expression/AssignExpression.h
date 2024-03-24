#ifndef HKNI_ASSIGN_EXPRESSION_H
#define HKNI_ASSIGN_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"
#include "IdentifierExpression.h"

namespace asthkni {
    //赋值表达式
    //格式：标识符表达式 操作符 表达式
    class AssignExpression : public I9Expression {
    public:
        Token TokenHKNI; //就是操作符的字符串
        IdentifierExpression *I9NameExp; //标识符表达式
        string operationStr; //操作符（=、+=、-=、*=、/=、%=）
        I9Expression *I9ValueExp; //值表达式，需要先解释出来
    public:
        AssignExpression(Token token) {
            this->TokenHKNI = token;
            this->I9NameExp = nullptr;
            this->operationStr = token.Literal;
            this->I9ValueExp = nullptr;
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            string t4str;
            t4str.append(this->I9NameExp->ToString());
            t4str.append(" " + this->operationStr + " ");
            t4str.append(this->I9ValueExp->ToString());
            return t4str;
        }

        void ExpressionNode() override {}
    };
}

#endif
