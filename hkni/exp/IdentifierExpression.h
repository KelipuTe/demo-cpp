#ifndef HKNI_IDENTIFIER_EXPRESSION_H
#define HKNI_IDENTIFIER_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"

namespace ast {
    //标识符表达式
    class IdentifierExpression : public I9Expression {
        //####属性
    public:
        Token TokenHKNI;
        string Value;
        //####方法
    public:
        IdentifierExpression(Token token) {
            this->TokenHKNI = token;
            this->Value = token.Literal;
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

#endif //HKNI_IDENTIFIER_EXPRESSION_H
