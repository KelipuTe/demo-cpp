#ifndef DEMO_CPP_EXPRESSIONSTATEMENT_H
#define DEMO_CPP_EXPRESSIONSTATEMENT_H

#include <string>
#include "../Token.h"
#include "Expression.h"
#include "Statement.h"

namespace ast {
    class ExpressionStatement : public I9Statement {
        //####属性
    public:
        Token token;
        I9Expression *I9Exp;
        //####方法
    public:
        ExpressionStatement(Token token) {
            this->token = token;
        };

        string GetTokenLiteral() override {
            return this->token.Literal;
        }

        string ToString() override {
            string t4str;
            if (this->I9Exp != nullptr) {
                t4str.append(this->I9Exp->ToString());
                t4str.append(";");
            }
            return t4str;
        }

        void StatementNode() override {}
    };
}

#endif //DEMO_CPP_EXPRESSIONSTATEMENT_H
