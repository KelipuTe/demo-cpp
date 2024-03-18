#ifndef HKNI_BUILTIN_FUNC_OBJ_H
#define HKNI_BUILTIN_FUNC_OBJ_H

#include <functional>
#include "Object.h"

using namespace std;

namespace objecthkni {
    //内置函数
    class BuiltinFuncObject : public Object {
    public:
        ObjectType ValueObjectType;
        std::function<Object *(std::vector<Object *>)> _func;
    public:
        BuiltinFuncObject() {
            ValueObjectType = BUILTIN_FUNC_OBJ;
        }

        ObjectType GetObjectType() override {
            return ValueObjectType;
        }

        string GetLiteral() override {
            return "Builtin Function";
        }

        string ToString() override {

        }

    };

}

#endif
