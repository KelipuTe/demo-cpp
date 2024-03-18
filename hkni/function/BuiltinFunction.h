#ifndef HKNI_BUILTIN_FUNC_H
#define HKNI_BUILTIN_FUNC_H

#include <functional>
#include <map>

#include "../object/BuiltinFuncObject.h"
#include "PrintlnFunction.h"

using namespace std;
using namespace objecthkni;

namespace functionhkni {
    //内置函数
    class BuiltinFunc {
    public:
        std::map<string, BuiltinFuncObject *> _builtinFunc;
    public:
        BuiltinFunc() {
            initBuiltinFunc();
        }

        void initBuiltinFunc() {
            BuiltinFuncObject *printFunc = new BuiltinFuncObject();
            printFunc->_func = PrintlnFunc;
            _builtinFunc["println"] = printFunc;
        }

        BuiltinFuncObject *getBuiltinFunc(string funcName) {
            return _builtinFunc[funcName];
        }
    };
}

#endif
