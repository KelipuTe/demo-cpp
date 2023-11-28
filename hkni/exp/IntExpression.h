#ifndef HKNI_INT_EXPRESSION_H
#define HKNI_INT_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"

namespace ast {
    //整数表达式
    class IntExpression : public I9Expression {
        //####属性
    public:
        Token TokenHKNI;
        //####方法
    public:
        IntExpression(Token token) {
            this->TokenHKNI = token;
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            return this->TokenHKNI.Literal;
        }

        void ExpressionNode() override {}
    };
}

#endif //HKNI_INT_EXPRESSION_H
