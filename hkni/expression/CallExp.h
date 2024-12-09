#ifndef HKNI_CALL_EXP_H
#define HKNI_CALL_EXP_H

#include <string>
#include <vector>

#include "../ast/Expression.h"

namespace hkni {
    //函数调用表达式
    class CallExp : public ExpressionI9 {
    public:
        ExpressionI9* I9Exp;
        std::vector<ExpressionI9*> ArgExpList; //参数列表
    public:
        CallExp() {};

        string GetTokenLiteral() override {
            return "call";
        }

        string ToString() override {
            string t4str;
            t4str.append(I9Exp->ToString());
            t4str.append("(");

            int argNum = (int)ArgExpList.size();
            if(argNum>0){
                for (int index=0;index<argNum;index++) {
                    t4str.append(ArgExpList[index]->ToString());
                    if(index<argNum-1){
                        t4str.append(",");
                    }
                }
            }

            t4str.append(")");

            return t4str;
        }

        void ExpressionNode() override {}
    };
}

#endif
