#ifndef HKNI_VAR_STMT_H
#define HKNI_VAR_STMT_H

#include "../expression/IdentifierExp.h"
#include "../ast/Statement.h"

namespace hkni {
    //变量声明语句
    //只声明不赋值：var 标识符表达式 数据类型;
    //声明并赋值：var 标识符表达式 数据类型 = 值的表达式;
    class VarStmt : public StatementI9 {
    public:
        IdentifierExp *IdentifierExp; //标识符表达式
        Token TypeToken; //数据类型的token
        TokenType TypeTokenType; //数据类型的token
        ExpressionI9 *TypeExp; //数据类型表达式。数组、map，会用到
        ExpressionI9 *ValExp; //值的表达式，需要先解释出来

    public:
        VarStmt() {
            IdentifierExp = nullptr;
            TypeExp = nullptr;
            ValExp = nullptr;
        };

        string GetTokenLiteral() override {
            return TypeToken.Literal;
        }

        string ToString() override {
            string str;

            str.append("var ");
            str.append(IdentifierExp->ToString());
            str.append(" " + TypeToken.Literal);
            if (ValExp != nullptr) {
                str.append(" = ");
                str.append(ValExp->ToString());
            }

            return str;
        }

        void StatementNode() override {}
    };
}

#endif
