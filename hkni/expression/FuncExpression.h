#ifndef HKNI_FUNC_EXPRESSION_H
#define HKNI_FUNC_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"
#include "../expression/IdentifierExpression.h"
#include "../statement/BlockStatement.h"

namespace ast {
    //函数声明表达式
    //格式：function 函数名(参数列表){块语句}
    class FuncExpression : public I9Expression {
        //##属性
    public:
        Token TokenHKNI;
        IdentifierExpression *P7NameExp; //函数名
        std::vector<TokenType> P7ArgTypeList; //参数类型列表
        std::vector<IdentifierExpression *> P7ArgList; //参数列表
        TokenType ReturnType; //返回类型
        BlockStatement *P7BodyBlockStmt; //函数体
        //##方法
    public:
        FuncExpression(Token token) {
            this->TokenHKNI = token;
            this->P7NameExp = nullptr;
        };

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            string str;
            str.append("function ");
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
            str.append(TokenTypeToString(ReturnType));
            str.append(" {\n");
            str.append(P7BodyBlockStmt->ToString());
            str.append("}\n");
            return str;
        }

        void ExpressionNode() override {}

    };
}
#endif
