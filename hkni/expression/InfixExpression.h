#ifndef HKNI_INFIX_EXPRESSION_H
#define HKNI_INFIX_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"

namespace asthkni {
    //中缀表达式
    //格式：表达式 操作符 表达式
    class InfixExpression : public ExpressionI9 {
    public:
        Token TokenHKNI; //操作符
        ExpressionI9 *I9LeftExp;
        string OperatorStr;
        ExpressionI9 *I9RightExp;
    public:
        InfixExpression(Token token) {
            this->TokenHKNI = token;
            this->I9LeftExp = nullptr;
            this->OperatorStr = token.Literal;
            this->I9RightExp = nullptr;
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            string t4str;
            t4str.append("(");
            t4str.append(this->I9LeftExp->ToString());
            t4str.append(" " + this->OperatorStr + " ");
            t4str.append(this->I9RightExp->ToString());
            t4str.append(")");
            return t4str;
        }

        void ExpressionNode() override {}
    };
}

#endif
