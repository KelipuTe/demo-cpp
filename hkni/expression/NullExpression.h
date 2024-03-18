#ifndef HKNI_NULL_EXPRESSION_H
#define HKNI_NULL_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"

namespace asthkni {
    //null表达式
    class NullExpression : public I9Expression {
        //##属性
    public:
        Token TokenHKNI;
        //##方法
    public:
        NullExpression(Token token) {
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

#endif
