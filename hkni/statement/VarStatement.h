#ifndef HKNI_VAR_STATEMENT_H
#define HKNI_VAR_STATEMENT_H

#include "../expression/IdentifierExpression.h"
#include "../ast/Statement.h"

namespace ast {
    //变量声明语句
    //格式：变量类型 标识符表达式; 变量类型 标识符表达式 = 表达式;
    class VarStatement : public I9Statement {
        //##属性
    public:
        IdentifierExpression *P7IdentifierExp; //标识符表达式
        TokenType ValueType; //值类型
        I9Expression *I9ValueExp; //值表达式，需要先解释出来
    private:
        Token token; //就是变量值类型的关键字
        //##方法
    public:
        VarStatement() {
            this->P7IdentifierExp = nullptr;
            this->I9ValueExp = nullptr;
        };

        void SetToken(Token t) {
            this->token = t;
            this->ValueType = t.TokenType;
        }

        string GetTokenLiteral() override {
            return token.Literal;
        }

        string ToString() override {
            string str;
            str.append("var ");
            str.append(P7IdentifierExp->ToString());
            str.append(" " + token.Literal);
            if (I9ValueExp != nullptr) {
                str.append(" = ");
                str.append(I9ValueExp->ToString());
            }
            str.append(";");
            return str;
        }

        void StatementNode() override {}
    };
}

#endif
