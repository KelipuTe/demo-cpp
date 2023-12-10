#ifndef HKNI_CALL_EXPRESSION_H
#define HKNI_CALL_EXPRESSION_H

#include <string>
#include <vector>
#include "../Token.h"
#include "../ast/Expression.h"

namespace ast {
    //函数调用表达式
    class CallExpression : public I9Expression {
        //####属性
    public:
        Token TokenHKNI;
        I9Expression* I9Exp;
        std::vector<I9Expression*> P7ArgList; //参数列表
        //####方法
    public:
        CallExpression(Token token) {
            this->TokenHKNI = token;
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            string t4str;
            t4str.append(I9Exp->ToString());
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

            t4str.append(")");

            return t4str;
        }

        void ExpressionNode() override {}
    };
}

#endif //HKNI_CALL_EXPRESSION_H
