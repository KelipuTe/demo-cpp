#ifndef HKNI_FLOAT_EXPRESSION_H
#define HKNI_FLOAT_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "Expression.h"

namespace ast {
    class FloatExpression : public I9Expression {
        //####属性
    private:
        Token token;
        //####方法
    public:
        FloatExpression(Token token) {
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

#endif //HKNI_FLOAT_EXPRESSION_H
