#ifndef DEMO_CPP_PROGRAM_H
#define DEMO_CPP_PROGRAM_H

#include <vector>
#include "Node.h"
#include "Statement.h"

using namespace std;

namespace ast {
    //程序
    class Program : public I9Node {
    public:
        std::vector<I9Statement *> I9StatementList;
    public:
        string GetTokenLiteral() override {
            if (!I9StatementList.empty()) {
                return I9StatementList[0]->GetTokenLiteral();
            } else {
                return "";
            }
        }

        string ToString() override {
            string Result;
            for (auto &item: I9StatementList) {
                Result += item->ToString();
            }
            return Result;
        }
    };
}

#endif //DEMO_CPP_PROGRAM_H
