#ifndef HKNI_STRING_EXP_H
#define HKNI_STRING_EXP_H

#include <string>

#include "../Token.h"
#include "../ast/Expression.h"

namespace hkni {
    //字符串表达式
    class StringExp : public ExpressionI9 {
    public:
        Token TokenHKNI;
    public:
        StringExp(Token t) {
            this->TokenHKNI = t;
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            string str;
            str.append("\"");
            str.append(this->TokenHKNI.Literal);
            str.append("\"");
            return str;
        }

        void ExpressionNode() override {}
    };
}

#endif
