#ifndef HKNI_PROGRAM_H
#define HKNI_PROGRAM_H

#include <vector>
#include "Statement.h"

using namespace std;

namespace ast {
    //程序
    class Program : public I9Node {
    public:
        std::vector<I9Statement *> I9StatementList; //语句列表
    public:
        string GetTokenLiteral() override {
            if (!I9StatementList.empty()) {
                return I9StatementList[0]->GetTokenLiteral();
            } else {
                return "";
            }
        }

        string ToString() override {
            string t4str;
            for (auto &item: I9StatementList) {
                t4str += item->ToString();
            }
            return t4str;
        }
    };
}

#endif //HKNI_PROGRAM_H
