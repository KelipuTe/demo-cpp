#ifndef HKNI_FLOAT_EXP_H
#define HKNI_FLOAT_EXP_H

#include <string>

#include "../Token.h"
#include "../ast/Expression.h"

namespace hkni {
    //浮点数表达式
    class FloatExp : public ExpressionI9 {
    public:
        Token TokenHKNI;
        float Value;

    public:
        FloatExp(Token token) {
            this->TokenHKNI = token;
            this->Value = stof(token.Literal);
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            return this->TokenHKNI.Literal;
        }

        void ExpressionNode() override {}

        float GetValue() {
            return this->Value;
        }
    };
}

#endif
