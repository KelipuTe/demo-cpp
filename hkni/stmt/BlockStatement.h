#ifndef HKNI_BLOCK_STATEMENT_H
#define HKNI_BLOCK_STATEMENT_H

#include <vector>
#include "../Token.h"
#include "../ast/Statement.h"

namespace ast {
    //块语句
    class BlockStatement : public I9Statement {
        //####属性
    public:
        Token TokenHKNI;
        std::vector<I9Statement*> BodyStmtList;
        //####方法
    public:
        BlockStatement(Token token) {
            this->TokenHKNI = token;
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
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

#endif //HKNI_BLOCK_STATEMENT_H
