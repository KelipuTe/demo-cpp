#ifndef HKNI_BOOL_EXP_H
#define HKNI_BOOL_EXP_H

#include <string>

#include "../Token.h"
#include "../ast/Expression.h"

namespace hkni {
    //布尔表达式
    class BoolExp : public ExpressionI9 {
    public:
        Token token; //就是true或false
    public:
        BoolExp(Token token) {
            this->token = token;
        };

        TokenType GetTokenType(){
            return token.TokenType;
        }

        string GetTokenLiteral() override {
            return this->token.Literal;
        }

        string ToString() override {
            return this->token.Literal;
        }

        void ExpressionNode() override {}
    };
}

#endif
