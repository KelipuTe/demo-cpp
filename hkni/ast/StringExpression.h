#ifndef HKNI_STRING_EXPRESSION_H
#define HKNI_STRING_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "Expression.h"

namespace ast {
    class StringExpression : public I9Expression {
        //####属性
    public:
        Token token;
        //####方法
    public:
        StringExpression(Token token) {
            this->token = token;
        };

        string GetTokenLiteral() override {
            return this->token.Literal;
        }

        string ToString() override {
            string t4str;
            t4str.append("\"");
                t4str.append(this->token.Literal);
            t4str.append("\"");
            return t4str;
        }

        void ExpressionNode() override {}
    };
}

#endif //HKNI_STRING_EXPRESSION_H
