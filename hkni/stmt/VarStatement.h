#ifndef HKNI_VAR_STATEMENT_H
#define HKNI_VAR_STATEMENT_H

#include "../exp/IdentifierExpression.h"
#include "../ast/Statement.h"

namespace ast {
    //变量声明语句
    //格式：变量类型 标识符表达式; 变量类型 标识符表达式 = 表达式;
    class VarStatement : public I9Statement {
        //####属性
    public:
        TOKEN_TYPE ValueType; //变量类型
        IdentifierExpression *P7NameExp; //标识符表达式
        I9Expression *I9ValueExp; //值表达式，需要先解释出来
    private:
        Token token; //就是变量类型的关键字
        //####方法
    public:
        VarStatement(Token token) {
            this->token = token;
            this->ValueType = token.TokenType;
            this->P7NameExp = nullptr;
            this->I9ValueExp = nullptr;
        };

        string GetTokenLiteral() override {
            return token.Literal;
        }

        string ToString() override {
            string t4str;
            t4str.append(token.Literal + " ");
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
