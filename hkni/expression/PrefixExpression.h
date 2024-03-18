#ifndef HKNI_PREFIX_EXPRESSION_H
#define HKNI_PREFIX_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"

namespace asthkni {
    //前缀表达式
    //格式：操作符 表达式
    class PrefixExpression : public I9Expression {
        //##属性
    public:
        Token TokenHKNI;
        string OperatorStr;
        I9Expression *I9RightExp;
        //##方法
    public:
        PrefixExpression(Token token) {
            this->TokenHKNI = token;
            this->OperatorStr = token.Literal;
            this->I9RightExp = nullptr;
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            string t4str;
            t4str.append("(");
            t4str.append(this->OperatorStr);
            t4str.append(this->I9RightExp->ToString());
            t4str.append(")");
            return t4str;
        }

        void ExpressionNode() override {}
    };
}

#endif
