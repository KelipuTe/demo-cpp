#ifndef HKNI_STRUCT_EXPRESSION_H
#define HKNI_STRUCT_EXPRESSION_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"
#include "StructField.h"

using namespace hkni;

namespace asthkni {
    //结构体
    class StructExpression : public ExpressionI9 {
    public:
        IdentifierExpression *NameP7; //结构体标识符
        std::map<string, StructField*> FieldP7Map; //结构体字段
        std::map<string, ExpressionI9 *> MethodP7Map; //结构体函数
    public:
        StructExpression() {};

        string GetTokenLiteral() override {
            return "StructExpression";
        }

        string ToString() override {
            return "StructExpression";
        }

        void ExpressionNode() override {}
    };
}

#endif
