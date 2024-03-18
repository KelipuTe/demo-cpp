#ifndef HKNI_FUNC_OBJECT_H
#define HKNI_FUNC_OBJECT_H

#include "Object.h"

#include "../expression/IdentifierExpression.h"
#include "../statement/BlockStatement.h"

#include "../Environment.h"

using namespace asthkni;

namespace objecthkni {
    class FuncObject : public Object {
        //##属性
    public:
        ObjectType ValueObjectType;
        IdentifierExpression *P7NameExp; //函数名
        std::vector<IdentifierExpression*> P7ArgList; //参数列表
        BlockStatement* P7BodyBlockStmt; //函数体
        Environment *p7env; //函数作用域
        //##方法
    public:
        FuncObject() {
            ValueObjectType = FUNC_OBJ;
        }

        ObjectType GetObjectType() override {
            return ValueObjectType;
        }

        string GetLiteral() override {
        }

        string ToString() override {
            string t4str;
            return t4str;
        }
    };
}

#endif
