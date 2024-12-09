#ifndef HKNI_FOR_EXP_H
#define HKNI_FOR_EXP_H

#include <string>

#include "../ast/Expression.h"
#include "../statement/BlockStmt.h"

namespace hkni {
    //for表达式
    //格式：for(初始化语句;条件表语句;递增递减语句){块语句}
    class ForExp : public ExpressionI9 {
    public:
        ExpressionI9 *I9InitExp; //初始化
        ExpressionI9 *I9ConditionExp; //条件
        ExpressionI9 *I9IncrementExp; //递增递减
        BlockStmt *P7BodyStmt; //循环块
    public:
        ForExp() {
            I9InitExp = nullptr;
            I9ConditionExp = nullptr;
            I9IncrementExp = nullptr;
            P7BodyStmt = nullptr;
        };

        string GetTokenLiteral() override {
            return "for";
        }

        string ToString() override {
            string str;
            str.append("for (");

            str.append(I9InitExp->ToString());
            str.append(I9ConditionExp->ToString());
            str.append(I9IncrementExp->ToString());

            size_t start_pos = 0;
            while((start_pos = str.find('\n', start_pos)) != std::string::npos) {
                str.replace(start_pos, 1, "");
            }

            str.append(") {\n");
            if(P7BodyStmt!= nullptr) {
                str.append(P7BodyStmt->ToString());
            }
            str.append("}");

            return str;
        }

        void ExpressionNode() override {}
    };
}

#endif
