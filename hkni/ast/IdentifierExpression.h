#ifndef DEMO_CPP_IDENTIFIEREXPRESSION_H
#define DEMO_CPP_IDENTIFIEREXPRESSION_H

#include <string>
#include "../Token.h"
#include "Expression.h"

namespace ast {
    //标识符表达式
    class IdentifierExpression : public I9Expression {
        //####属性
    private:
        Token token;
        //####方法
    public:
        IdentifierExpression(Token token) {
            this->token = token;
        };

        string GetTokenLiteral() override {
            return this->token.Literal;
        }

        string ToString() override {
            return this->token.Literal;
        }

        void ExpressionNode() override {
        }
    };
}

#endif //DEMO_CPP_IDENTIFIEREXPRESSION_H
