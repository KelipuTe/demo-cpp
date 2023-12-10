#ifndef HKNI_FUNC_EXPRESSION_H
#define HKNI_FUNC_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"
#include "../exp/IdentifierExpression.h"
#include "../stmt/BlockStatement.h"

namespace ast {
    //函数声明表达式
    //格式：func 函数名(参数列表){块语句}
    class FuncExpression : public I9Expression {
        //####属性
    public:
        Token TokenHKNI;
        IdentifierExpression *P7NameExp; //函数名
        std::vector<IdentifierExpression*> P7ArgList; //参数列表
        BlockStatement* P7BodyBlockStmt; //函数体
        //####方法
    public:
        FuncExpression(Token token) {
            this->TokenHKNI = token;
            this->P7NameExp= nullptr;
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            string t4str;
            t4str.append("func ");
            t4str.append(P7NameExp->ToString());
            t4str.append("(");

            int argNum = (int)P7ArgList.size();
            if(argNum>0){
                for (int index=0;index<argNum;index++) {
                    t4str.append(P7ArgList[index]->ToString());
                    if(index<argNum-1){
                        t4str.append(",");
                    }
                }
            }

            t4str.append("){\n");
            t4str.append(P7BodyBlockStmt->ToString());
            t4str.append("\n}\n");

            return t4str;
        }

        void ExpressionNode() override {}

    };
}
#endif //HKNI_FUNC_EXPRESSION_H
