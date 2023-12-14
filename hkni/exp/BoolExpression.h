#ifndef HKNI_BOOL_EXPRESSION_H
#define HKNI_BOOL_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"

namespace ast {
    //布尔表达式
    class BoolExpression : public I9Expression {
        //####属性
    public:
        Token token; //就是true或false
        //####方法
    public:
        BoolExpression(Token token) {
            this->token = token;
        };

        TOKEN_TYPE GetTokenType(){
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

#endif //HKNI_BOOL_EXPRESSION_H
