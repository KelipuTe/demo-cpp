#ifndef HKNI_STRING_EXPRESSION_H
#define HKNI_STRING_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"

namespace ast {
    //字符串表达式
    class StringExpression : public I9Expression {
    public:
        Token TokenHKNI;
    public:
        StringExpression(Token t) {
            this->TokenHKNI = t;
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            string str;
            str.append("\"");
            str.append(this->TokenHKNI.Literal);
            str.append("\"");
            return str;
        }

        void ExpressionNode() override {}
    };
}

#endif
