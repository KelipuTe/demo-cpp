#ifndef HKNI_SUFFIX_EXPRESSION_H
#define HKNI_SUFFIX_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"

namespace asthkni {
    //后缀表达式
    //格式：表达式 操作符
    class SuffixExpression : public I9Expression {
        //##属性
    public:
        Token TokenHKNI;
        I9Expression *I9Exp;
        string Operation;
        //##方法
    public:
        SuffixExpression(Token token) {
            this->TokenHKNI = token;
            this->I9Exp = nullptr;
            this->Operation = token.Literal;
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            string t4str;
            t4str.append("(");
            t4str.append(this->I9Exp->ToString());
            t4str.append(this->Operation);
            t4str.append(")");
            return t4str;
        }

        void ExpressionNode() override {}
    };
}

#endif
