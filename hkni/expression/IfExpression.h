#ifndef HKNI_IF_EXPRESSION_H
#define HKNI_IF_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"
#include "../statement/BlockStatement.h"

namespace asthkni {
    //if表达式
    //格式：if(条件表达式){块语句};
    //格式：if(条件表达式){块语句}else{块语句};
    class IfExpression : public I9Expression {
    public:
        I9Expression *I9ConditionExp; //条件
        BlockStatement *P7ConsequenceStmt; //if块
        BlockStatement *P7AlternativeStmt; //else块

    public:
        IfExpression() {
            I9ConditionExp = nullptr;
            P7ConsequenceStmt = nullptr;
            P7AlternativeStmt = nullptr;
        };

        string GetTokenLiteral() override {
            return "if";
        }

        string ToString() override {
            string str;
            str.append("if (");
            str.append(I9ConditionExp->ToString());
            str.append(") {\n");
            str.append(P7ConsequenceStmt->ToString());
            if (P7AlternativeStmt == nullptr) {
                str.append("}");
            } else {
                str.append("} else {\n");
                str.append(P7AlternativeStmt->ToString());
                str.append("}");
            }
            return str;
        }

        void ExpressionNode() override {}
    };
}

#endif
