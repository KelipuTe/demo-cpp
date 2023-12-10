#ifndef HKNI_IF_EXPRESSION_H
#define HKNI_IF_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"
#include "../stmt/BlockStatement.h"

namespace ast {
    //if表达式
    //格式：
    //1、if(条件表达式){块语句}
    //2、if(条件表达式){块语句}else{块语句}
    class IfExpression : public I9Expression {
        //####属性
    public:
        Token TokenHKNI;
        I9Expression *I9ConditionExp; //条件
        BlockStatement *P7ConsequenceStmt; //if块
        BlockStatement *P7AlternativeStmt; //else块
        //####方法
    public:
        IfExpression(Token token) {
            this->TokenHKNI = token;
            this->I9ConditionExp = nullptr;
            this->P7ConsequenceStmt = nullptr;
            this->P7AlternativeStmt = nullptr;
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            string t4str;
            t4str.append("if (");
            t4str.append(I9ConditionExp->ToString());
            t4str.append(") {\n");
            t4str.append(P7ConsequenceStmt->ToString());
            if (P7AlternativeStmt == nullptr) {
                t4str.append("\n}\n");
            } else {
                t4str.append("\n} else {\n");
                t4str.append(P7AlternativeStmt->ToString());
                t4str.append("\n}\n");
            }
            return t4str;
        }

        void ExpressionNode() override {}
    };
}

#endif //HKNI_IF_EXPRESSION_H
