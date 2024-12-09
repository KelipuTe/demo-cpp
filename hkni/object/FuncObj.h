#ifndef HKNI_FUNC_OBJ_H
#define HKNI_FUNC_OBJ_H

#include "Object.h"

#include "../expression/IdentifierExp.h"
#include "../statement/BlockStmt.h"

#include "../Environment.h"

namespace hkni {
    class FuncObj : public Object {
    public:
        ObjectType ValueObjectType;
        IdentifierExp *P7NameExp; //函数名
        std::vector<IdentifierExp*> P7ArgList; //参数列表
        BlockStmt* P7BodyBlockStmt; //函数体
        Environment *p7env; //函数作用域

    public:
        FuncObj() {
            ValueObjectType = FUNC_OBJ;
        }

        ObjectType GetObjectType() override {
            return ValueObjectType;
        }

        string GetLiteral() override {
            return "FuncObject";
        }

        string ToString() override {
            return "Object;ObjectType=" + ObjectTypeToString(ValueObjectType) + ";";
        }
    };
}

#endif
