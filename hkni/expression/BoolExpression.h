#ifndef HKNI_BOOL_EXPRESSION_H
#define HKNI_BOOL_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"

namespace asthkni {
    //布尔表达式
    class BoolExpression : public ExpressionI9 {
    public:
        Token token; //就是true或false
    public:
        BoolExpression(Token token) {
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
