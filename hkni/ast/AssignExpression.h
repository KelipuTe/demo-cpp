#ifndef DEMO_CPP_ASSIGNEXPRESSION_H
#define DEMO_CPP_ASSIGNEXPRESSION_H

#include "Expression.h"
#include "IdentifierExpression.h"

namespace ast {
    //赋值表达式
    class AssignExpression : public I9Expression {
        //####属性
    public:
        Token token;
        //标识符 = 表达式
        I9Expression *i9NameExp;
        string operation;
        I9Expression *i9ValueExp;
        //####方法
    public:
        AssignExpression(Token token) {
            this->token = token;
            this->i9NameExp = nullptr;
            this->operation = '=';
            this->i9ValueExp = nullptr;
        };

        string GetTokenLiteral() override {
            return this->token.Literal;
        }

        string ToString() override {
            string t4str;
            t4str.append(this->i9NameExp->ToString());
            t4str.append(" " + this->operation + " ");
            t4str.append(this->i9ValueExp->ToString());
            t4str.append(";");
            return t4str;
        }

        void ExpressionNode() override {}
    };
}

#endif //DEMO_CPP_ASSIGNEXPRESSION_H
