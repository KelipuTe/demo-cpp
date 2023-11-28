#ifndef HKNI_EXPRESSION_STATEMENT_H
#define HKNI_EXPRESSION_STATEMENT_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"
#include "../ast/Statement.h"

namespace ast {
    //表达式语句
    //格式：表达式;
    class ExpressionStatement : public I9Statement {
        //####属性
    public:
        Token TokenHKNI;
        I9Expression *I9Exp;
        //####方法
    public:
        ExpressionStatement(Token token) {
            this->TokenHKNI = token;
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
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

#endif //HKNI_EXPRESSION_STATEMENT_H
