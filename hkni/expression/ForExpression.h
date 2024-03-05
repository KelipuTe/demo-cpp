
#ifndef HKNI_FOR_EXPRESSION_H
#define HKNI_FOR_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"
#include "../statement/BlockStatement.h"

namespace ast {
    //if表达式
    //格式：for(初始化语句;条件表语句;递增递减语句){块语句}
    class ForExpression : public I9Expression {
        //##属性
    public:
        Token TokenHKNI;
        I9Statement *I9InitStmt; //
        I9Statement *I9ConditionStmt; //
        I9Statement *I9IncrementStmt; //
        BlockStatement *P7BodyStmt; //循环块
        //##方法
    public:
        ForExpression(Token token) {
            this->TokenHKNI = token;
            this->I9InitStmt = nullptr;
            this->I9ConditionStmt = nullptr;
            this->I9IncrementStmt = nullptr;
            this->P7BodyStmt = nullptr;
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            string t4str;
            t4str.append("for (");
            t4str.append(I9InitStmt->ToString());
            t4str.append(";");
            t4str.append(I9ConditionStmt->ToString());
            t4str.append(";");
            t4str.append(I9IncrementStmt->ToString());
            t4str.append(") {\n");
            if(P7BodyStmt!= nullptr) {
                t4str.append(P7BodyStmt->ToString());
            }
            t4str.append("\n}\n");
            return t4str;
        }

        void ExpressionNode() override {}
    };
}

#endif
