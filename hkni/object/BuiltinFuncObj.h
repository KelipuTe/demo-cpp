#ifndef HKNI_BUILTIN_FUNC_OBJ_H
#define HKNI_BUILTIN_FUNC_OBJ_H

#include <functional>
#include "Object.h"

using namespace std;

namespace object {
    /**
     * 内置函数
     */
    class BuiltinFuncObj : public Object {
    public:
        OBJECT_TYPE Type;
        std::function<Object *(std::vector<Object *>)> _func;
    public:
        BuiltinFuncObj() {
            this->Type = BUILTIN_FUNC_OBJ;
        }

        OBJECT_TYPE GetType() override {
            return this->Type;
        }

        string GetLiteral() override {
            return "Builtin Function";
        }

        string ToString() override {

        }

    };

}

#endif
