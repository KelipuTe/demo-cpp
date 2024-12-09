#ifndef HKNI_NULL_EXP_H
#define HKNI_NULL_EXP_H

#include <string>

#include "../Token.h"
#include "../ast/Expression.h"

namespace hkni {
    //null表达式
    class NullExp : public ExpressionI9 {
    public:
        Token TokenHKNI;
    public:
        NullExp(Token token) {
            this->TokenHKNI = token;
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            return this->TokenHKNI.Literal;
        }

        void ExpressionNode() override {}
    };
}

#endif
