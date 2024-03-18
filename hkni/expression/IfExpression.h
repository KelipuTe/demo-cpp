#ifndef HKNI_IF_EXPRESSION_H
#define HKNI_IF_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"
#include "../statement/BlockStatement.h"

namespace asthkni {
    //if表达式
    //格式：if(条件表达式){块语句}; if(条件表达式){块语句}else{块语句};
    class IfExpression : public I9Expression {
        //##属性
    public:
        I9Expression *I9ConditionExp; //条件
        BlockStatement *P7ConsequenceStmt; //if块
        BlockStatement *P7AlternativeStmt; //else块
    private:
        Token token;
        //##方法
    public:
        IfExpression(Token token) {
            this->token = token;
            this->I9ConditionExp = nullptr;
            this->P7ConsequenceStmt = nullptr;
            this->P7AlternativeStmt = nullptr;
        };

        string GetTokenLiteral() override {
            return this->token.Literal;
        }

        string ToString() override {
            string t4str;
            t4str.append("if (");
            t4str.append(I9ConditionExp->ToString());
            t4str.append(") {\n");
            t4str.append(P7ConsequenceStmt->ToString());
            if (P7AlternativeStmt == nullptr) {
                t4str.append("}");
            } else {
                t4str.append("} else {\n");
                t4str.append(P7AlternativeStmt->ToString());
                t4str.append("}");
            }
            return t4str;
        }

        void ExpressionNode() override {}
    };
}

#endif
