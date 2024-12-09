#ifndef HKNI_EXP_STMT_H
#define HKNI_EXP_STMT_H

#include <string>

#include "../Token.h"
#include "../ast/Expression.h"
#include "../ast/Statement.h"

using namespace hkni;

namespace hkni {
    //表达式语句
    class ExpStmt final : public StatementI9 {
    public:
        Token TokenHKNI;
        ExpressionI9 *ExpI9;

    public:
        explicit ExpStmt(Token t) {
            this->TokenHKNI = t;
            this->ExpI9 = nullptr;
        }

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            string str;
            if (this->ExpI9 != nullptr) {
                str.append(this->ExpI9->ToString());
                str.append(";\n");
            }
            return str;
        }

        void StatementNode() override {
        }
    };
}

#endif
