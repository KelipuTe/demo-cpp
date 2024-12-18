#ifndef HKNI_INDEX_EXP_H
#define HKNI_INDEX_EXP_H

#include <string>

#include "../Token.h"
#include "../ast/Expression.h"

namespace hkni {
    //索引表达式
    class IndexExp : public ExpressionI9 {
    public:
        Token TokenHKNI;
        ExpressionI9 *I9LeftExp;
        ExpressionI9 *I9RightExp;

        IndexExp(Token t) {
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
            str.append("[");
            str.append(I9RightExp->ToString());
            str.append("])");

            return str;
        }

        virtual void ExpressionNode() override {}
    };
}

#endif
