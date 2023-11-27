#ifndef HKNI_INT_EXPRESSION_H
#define HKNI_INT_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "Expression.h"

namespace ast {
    class IntExpression : public I9Expression {
        //####属性
    private:
        Token token;
        //####方法
    public:
        IntExpression(Token token) {
            this->token = token;
        };

        string GetTokenLiteral() override {
            return this->token.Literal;
        }

        string ToString() override {
            return this->token.Literal;
        }

        void ExpressionNode() override {}
    };
}

#endif //HKNI_INT_EXPRESSION_H
