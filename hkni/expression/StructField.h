#ifndef HKNI_STRUCT_FIELD_H
#define HKNI_STRUCT_FIELD_H

#include <string>

#include "../Token.h"
#include "../ast/Expression.h"

namespace hkni {
    //结构体字段
    class StructField : public ExpressionI9 {
    public:
        ExpressionI9 *FieldP7; //字段标识符
        TokenType FieldType; //字段类型
    public:
        StructField() {};

        string GetTokenLiteral() override {
            return "StructField:" + FieldP7->GetTokenLiteral();
        }

        string ToString() override {
            string str;
            str.append(FieldP7->ToString());
            str.append(" ");
            str.append(TokenTypeToString(FieldType));
            str.append(",\n");
            return str;
        }

        void ExpressionNode() override {}
    };
}

#endif
