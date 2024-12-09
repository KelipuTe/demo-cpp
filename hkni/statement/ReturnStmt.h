#ifndef HKNI_RETURN_STMT_H
#define HKNI_RETURN_STMT_H

#include <string>

#include "../Token.h"
#include "../ast/Expression.h"
#include "../ast/Statement.h"

namespace hkni {
    //return语句
    //格式：return 表达式;
    class ReturnStmt : public StatementI9 {
        //##属性
    public:
        ExpressionI9 *I9Exp;
    private:
        Token token; //就是return关键字
        //##方法
    public:
        ReturnStmt(Token token) {
            this->token = token;
        };

        string GetTokenLiteral() override {
            return this->token.Literal;
        }

        string ToString() override {
            string str;
            str.append("return");
            if (this->I9Exp != nullptr) {
                str.append(" ");
                str.append(this->I9Exp->ToString());
            }
            str.append(";\n");
            return str;
        }

        void StatementNode() override {}
    };
}

#endif
