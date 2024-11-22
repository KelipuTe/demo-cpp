#ifndef HKNI_VAR_STATEMENT_H
#define HKNI_VAR_STATEMENT_H

#include "../expression/IdentifierExpression.h"
#include "../ast/Statement.h"

namespace asthkni {
    //变量声明语句
    //格式：var 标识符表达式 变量类型;
    //格式：var 标识符表达式 变量类型 = 表达式;
    class VarStatement : public StatementI9 {
    public:
        IdentifierExpression *P7IdentifierExp; //标识符表达式
        Token ValueToken; //变量类型的token
        TokenType ValueTokenType; //变量类型的token
        ExpressionI9 *I9ValueExp; //值表达式，需要先解释出来
    public:
        VarStatement() {
            this->P7IdentifierExp = nullptr;
            this->I9ValueExp = nullptr;
        };

        string GetTokenLiteral() override {
            return ValueToken.Literal;
        }

        string ToString() override {
            string str;
            str.append("var ");
            str.append(P7IdentifierExp->ToString());
            str.append(" " + ValueToken.Literal);
            if (I9ValueExp != nullptr) {
                str.append(" = ");
                str.append(I9ValueExp->ToString());
            }
            str.append(";\n");
            return str;
        }

        void StatementNode() override {}
    };
}

#endif
