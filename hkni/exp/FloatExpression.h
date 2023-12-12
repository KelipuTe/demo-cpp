#ifndef HKNI_FLOAT_EXPRESSION_H
#define HKNI_FLOAT_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"

namespace ast {
    //浮点数表达式
    class FloatExpression : public I9Expression {
        //####属性
    public:
        Token TokenHKNI;
        float Value;
        //####方法
    public:
        FloatExpression(Token token) {
            this->TokenHKNI = token;
            this->Value = stof(token.Literal);
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            return this->TokenHKNI.Literal;
        }

        void ExpressionNode() override {}

        float GetValue() {
            return this->Value;
        }
    };
}

#endif //HKNI_FLOAT_EXPRESSION_H
