#ifndef HKNI_FUNC_EXP_H
#define HKNI_FUNC_EXP_H

#include <string>

#include "../Token.h"
#include "../ast/Expression.h"
#include "IdentifierExp.h"
#include "../statement/BlockStmt.h"

namespace hkni {
    //函数声明表达式
    //func 函数名(参数列表) 返回类型 {块语句}
    //参数列表：参数名 数据类型, 参数名 数据类型,...
    class FuncExp : public ExpressionI9 {
        //##属性
    public:
        IdentifierExp *P7NameExp; //函数名
        std::vector<IdentifierExp *> P7ArgList; //参数名列表
        std::vector<TokenType> P7ArgTypeList; //参数类型列表
        TokenType ReturnType; //返回类型
        BlockStmt *P7BodyBlockStmt; //函数体
        //##方法
    public:
        FuncExp() {
            P7NameExp = nullptr;
            ReturnType = NULL_HKNI;
        };

        string GetTokenLiteral() override {
            return "func";
        }

        string ToString() override {
            string str;
            str.append("func ");
            str.append(P7NameExp->ToString());
            str.append("(");
            int argNum = (int) P7ArgList.size();
            if (argNum > 0) {
                for (int index = 0; index < argNum; index++) {
                    str.append(P7ArgList[index]->ToString());
                    str.append(" ");
                    str.append(TokenTypeToString(P7ArgTypeList[index]));
                    if (index < argNum - 1) {
                        str.append(",");
                    }
                }
            }
            str.append(") ");
            if (ReturnType != ILLEGAL) {
                str.append(TokenTypeToString(ReturnType) + " ");
            }
            str.append("{\n");
            str.append(P7BodyBlockStmt->ToString());
            str.append("}");
            return str;
        }

        void ExpressionNode() override {}

    };
}
#endif
