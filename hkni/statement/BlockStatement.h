#ifndef HKNI_BLOCK_STATEMENT_H
#define HKNI_BLOCK_STATEMENT_H

#include <vector>
#include "../Token.h"
#include "../ast/Statement.h"

namespace asthkni {
    //块语句
    class BlockStatement : public I9Statement {
    public:
        std::vector<I9Statement*> BodyStmtList;

    public:
        BlockStatement() {};

        string GetTokenLiteral() override {
            return "BlockStatement";
        }

        string ToString() override {
            string str;
            for (auto &item: BodyStmtList) {
                str += item->ToString();
            }
            return str;
        }

        void StatementNode() override {}
    };
}

#endif
