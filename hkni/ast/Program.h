#ifndef HKNI_PROGRAM_H
#define HKNI_PROGRAM_H

#include <vector>
#include "Statement.h"

using namespace std;

namespace asthkni {
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
            string strT4;
            for (auto &item: StmtI9List) {
                strT4 += item->ToString();
            }
            return strT4;
        }
    };
}

#endif
