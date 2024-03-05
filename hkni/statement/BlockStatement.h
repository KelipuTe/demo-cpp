#ifndef HKNI_BLOCK_STATEMENT_H
#define HKNI_BLOCK_STATEMENT_H

#include <vector>
#include "../Token.h"
#include "../ast/Statement.h"

namespace ast {
    //块语句
    class BlockStatement : public I9Statement {
        //##属性
    public:
        std::vector<I9Statement*> BodyStmtList;
    private:
        Token token;
        //##方法
    public:
        BlockStatement(Token token) {
            this->token = token;
        };

        string GetTokenLiteral() override {
            return this->token.Literal;
        }

        string ToString() override {
            string t4str;
            for (auto &item: BodyStmtList) {
                t4str += item->ToString();
            }
            return t4str;
        }

        void StatementNode() override {}
    };
}

#endif
