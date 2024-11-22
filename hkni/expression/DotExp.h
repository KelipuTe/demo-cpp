
#ifndef HKNI_DOT_EXP_H
#define HKNI_DOT_EXP_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"
using namespace hkni;

namespace asthkni {
    //点引表达式
    class DotExp : public ExpressionI9 {
    public:
        Token TokenHKNI;
        ExpressionI9 *I9LeftExp;
        ExpressionI9 *I9RightExp;

        DotExp(Token t) {
            this->TokenHKNI = t;
            this->I9LeftExp = nullptr;
            this->I9RightExp = nullptr;
        }

        virtual string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        virtual string ToString() override {
            string str;

            str.append("(");
            str.append(I9LeftExp->ToString());
            str.append(".");
            str.append(I9RightExp->ToString());
            str.append(")");

            return str;
        }

        virtual void ExpressionNode() override {}
    };
}

#endif
