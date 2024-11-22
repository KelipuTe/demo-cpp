#ifndef HKNI_INT_EXPRESSION_H
#define HKNI_INT_EXPRESSION_H

#include <string>

#include "../Token.h"
#include "../ast/Expression.h"

namespace asthkni {
    //整数表达式
    class IntExpression : public ExpressionI9 {
    public:
        Token TokenHKNI;
        int Value;
    public:
        IntExpression(Token token) {
            this->TokenHKNI = token;
            this->Value = stoi(token.Literal);
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            return this->TokenHKNI.Literal;
        }

        void ExpressionNode() override {}

        int GetValue() {
            return this->Value;
        }
    };
}

#endif
