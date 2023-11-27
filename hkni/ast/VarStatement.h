#ifndef DEMO_CPP_VARSTATEMENT_H
#define DEMO_CPP_VARSTATEMENT_H

#include "IdentifierExpression.h"
#include "Statement.h"

namespace ast {
    class VarStatement : public I9Statement {
        //####属性
    public:
        IdentifierExpression *p7NameExp;
        I9Expression *i9ValueExp;
    private:
        Token token;
        //####方法
    public:
        VarStatement(Token token) {
            this->token = token;
            this->p7NameExp = nullptr;
            this->i9ValueExp = nullptr;
        };

        string GetTokenLiteral() override {
            return this->token.Literal;
        }

        string ToString() override {
            string t4str;
            t4str.append(token.Literal + " ");
            t4str.append(p7NameExp->ToString());
            if (i9ValueExp != nullptr) {
                t4str.append(" = ");
                t4str.append(i9ValueExp->ToString());
            }
            t4str.append(";");
            return t4str;
        }

        void StatementNode() override {}
    };
}

#endif //DEMO_CPP_VARSTATEMENT_H
