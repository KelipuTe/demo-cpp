#ifndef HKNI_TYPE_EXPRESSION_H
#define HKNI_TYPE_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"

using namespace hkni;

//type 类型标识符 类型关键字 {具体定义}
//类型关键字：struct

namespace asthkni {
    //类型定义
    class TypeExpression : public ExpressionI9 {
    public:
        TokenType RealType; //类型关键字
        ExpressionI9 *RealExpP7; //具体定义
    public:
        TypeExpression() {};

        string GetTokenLiteral() override {
            return "TypeExpression";
        }

        string ToString() override {
            return "TypeExpression";
        }

        void ExpressionNode() override {}
    };
}

#endif
