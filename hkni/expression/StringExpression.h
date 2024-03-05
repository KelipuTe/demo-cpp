#ifndef HKNI_STRING_EXPRESSION_H
#define HKNI_STRING_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"

namespace ast {
    //字符串表达式
    class StringExpression : public I9Expression {
        //##属性
    public:
        Token TokenHKNI;
        //##方法
    public:
        StringExpression(Token token) {
            this->TokenHKNI = token;
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            string t4str;
            t4str.append("\"");
            t4str.append(this->TokenHKNI.Literal);
            t4str.append("\"");
            return t4str;
        }

        void ExpressionNode() override {}
    };
}

#endif
