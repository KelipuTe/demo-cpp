#ifndef HKNI_IDENTIFIER_EXP_H
#define HKNI_IDENTIFIER_EXP_H

#include <string>

#include "../Token.h"
#include "../ast/Expression.h"

namespace hkni {
    //标识符表达式
    class IdentifierExp : public ExpressionI9 {
    public:
        Token Name;
        Token Type;
    public:
        IdentifierExp(Token t) {
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
