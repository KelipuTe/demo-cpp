#ifndef HKNI_VAR_STATEMENT_H
#define HKNI_VAR_STATEMENT_H

#include "../exp/IdentifierExpression.h"
#include "../ast/Statement.h"

namespace ast {
    //变量声明语句
    //格式：var 标识符表达式 = 表达式;
    class VarStatement : public I9Statement {
        //####属性
    public:
        Token TokenHKNI;
        IdentifierExpression *P7NameExp;
        I9Expression *I9ValueExp;
        //####方法
    public:
        VarStatement(Token token) {
            this->TokenHKNI = token;
            this->P7NameExp = nullptr;
            this->I9ValueExp = nullptr;
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            string t4str;
            t4str.append(TokenHKNI.Literal + " ");
            t4str.append(P7NameExp->ToString());
            if (I9ValueExp != nullptr) {
                t4str.append(" = ");
                t4str.append(I9ValueExp->ToString());
            }
            return t4str;
        }

        void StatementNode() override {}
    };
}

#endif //HKNI_VAR_STATEMENT_H
