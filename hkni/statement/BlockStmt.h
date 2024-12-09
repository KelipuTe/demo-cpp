#ifndef HKNI_BLOCK_STMT_H
#define HKNI_BLOCK_STMT_H

#include <vector>

#include "../ast/Statement.h"

namespace hkni {
    //块语句
    class BlockStmt : public StatementI9 {
    public:
        std::vector<StatementI9*> BodyStmtList;

    public:
        BlockStmt() {};

        string GetTokenLiteral() override {
            return "BlockStmt";
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
