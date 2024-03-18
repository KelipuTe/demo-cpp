#ifndef HKNI_EXPRESSION_STATEMENT_H
#define HKNI_EXPRESSION_STATEMENT_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"
#include "../ast/Statement.h"

using namespace hkni;

namespace asthkni {
    //表达式语句
    class ExpressionStatement : public I9Statement {
        //##属性
    public:
        Token TokenHKNI;
        I9Expression *I9Exp;
        //##方法
    public:
        ExpressionStatement(Token t) {
            this->TokenHKNI = t;
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            string str;
            if (this->I9Exp != nullptr) {
                str.append(this->I9Exp->ToString());
                str.append(";\n");
            }
            return str;
        }

        void StatementNode() override {}
    };
}

#endif
