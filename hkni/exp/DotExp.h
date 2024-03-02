
#ifndef HKNI_DOT_EXP_H
#define HKNI_DOT_EXP_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"

namespace ast {
    //点引表达式
    class DotExp : public I9Expression {
    public:
        Token TokenHKNI;
        I9Expression *I9LeftExp;
        I9Expression *I9RightExp;

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

#endif //HKNI_DOT_EXP_H
