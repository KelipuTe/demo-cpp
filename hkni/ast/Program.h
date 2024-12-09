#ifndef HKNI_PROGRAM_H
#define HKNI_PROGRAM_H

#include <vector>

#include "Statement.h"

namespace hkni {
    //程序
    class Program final : public NodeI9 {
    public:
        std::vector<StatementI9 *> StmtI9List; //语句列表（程序由语句组成）

        string GetTokenLiteral() override {
            if (!StmtI9List.empty()) {
                return StmtI9List[0]->GetTokenLiteral();
            }
            return "";
        }

        string ToString() override {
            string str;
            for (auto &item: StmtI9List) {
                str += item->ToString();
            }
            return str;
        }
    };
}

#endif
