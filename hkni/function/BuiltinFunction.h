#ifndef HKNI_BUILTIN_FUNC_H
#define HKNI_BUILTIN_FUNC_H

#include <functional>
#include <map>

#include "../object/BuiltinFuncObj.h"
#include "PrintlnFunction.h"

using namespace object;
using namespace std;

namespace function {
    /**
     * 内置函数
     */
    class BuiltinFunc  {
    public:
        std::map<string, BuiltinFuncObj*> _builtinFunc;
    public:
        BuiltinFunc(){
            initBuiltinFunc();
        }
        void initBuiltinFunc(){
            BuiltinFuncObj* printFunc = new BuiltinFuncObj();
            printFunc->_func=PrintlnFunc;
            _builtinFunc["println"] = printFunc;
        }

        BuiltinFuncObj* getBuiltinFunc(string funcName) {
            return _builtinFunc[funcName];
        }
    };
}

#endif
