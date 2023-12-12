#ifndef HKNI_ASSIGN_EXPRESSION_H
#define HKNI_ASSIGN_EXPRESSION_H

#include <string>
#include "../ast/Expression.h"

namespace ast {
    //赋值表达式
    //格式：标识符表达式 = 表达式
    class AssignExpression : public I9Expression {
        //####属性
    public:
        Token TokenHKNI;
        IdentifierExpression *I9NameExp;
        string Operation;
        I9Expression *I9ValueExp;
        //####方法
    public:
        AssignExpression(Token token) {
            this->TokenHKNI = token;
            this->I9NameExp = nullptr;
            this->Operation = token.Literal;
            this->I9ValueExp = nullptr;
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            string t4str;
            t4str.append(this->I9NameExp->ToString());
            t4str.append(" " + this->Operation + " ");
            t4str.append(this->I9ValueExp->ToString());
            return t4str;
        }

        void ExpressionNode() override {}
    };
}

#endif //HKNI_ASSIGN_EXPRESSION_H
