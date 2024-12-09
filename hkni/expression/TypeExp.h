#ifndef HKNI_TYPE_EXP_H
#define HKNI_TYPE_EXP_H

#include <string>

#include "../Token.h"
#include "../ast/Expression.h"

//type 类型标识符 类型关键字 {具体定义}
//类型关键字：struct

namespace hkni {
    //类型定义
    class TypeExp : public ExpressionI9 {
    public:
        TokenType RealType; //类型关键字
        ExpressionI9 *RealExpP7; //具体定义
    public:
        TypeExp() {};

        string GetTokenLiteral() override {
            return "TypeExp";
        }

        string ToString() override {
            return "TypeExp";
        }

        void ExpressionNode() override {}
    };
}

#endif
