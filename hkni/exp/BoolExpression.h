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
        Token TokenHKNI;
        //####方法
    public:
        BoolExpression(Token token) {
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

#endif //HKNI_BOOL_EXPRESSION_H
