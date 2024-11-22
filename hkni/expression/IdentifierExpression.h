#ifndef HKNI_IDENTIFIER_EXPRESSION_H
#define HKNI_IDENTIFIER_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"

using namespace hkni;

namespace asthkni {
    //标识符表达式
    class IdentifierExpression : public ExpressionI9 {
    public:
        Token Name;
        Token Type;
    public:
        IdentifierExpression(Token t) {
            Name = t;
        };

        string GetTokenLiteral() override {
            return Name.Literal;
        }

        string ToString() override {
            return Name.Literal;
        }

        void ExpressionNode() override {}
    };
}

#endif
