#ifndef HKNI_IDENTIFIER_EXPRESSION_H
#define HKNI_IDENTIFIER_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"

using namespace hkni;

namespace asthkni {
    //标识符表达式
    class IdentifierExpression : public I9Expression {
    public:
        Token TokenHKNI;

    public:
        IdentifierExpression(Token t) {
            TokenHKNI = t;
        };

        string GetTokenLiteral() override {
            return TokenHKNI.Literal;
        }

        string ToString() override {
            return TokenHKNI.Literal;
        }

        void ExpressionNode() override {}
    };
}

#endif
