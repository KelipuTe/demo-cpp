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
        //内置函数map，key为函数名，value为函数对象
        std::map<string, BuiltinFuncObject *> BuiltinFuncMap;

    public:
        BuiltinFunc() {
            InitBuiltinFunc();
        }

        void InitBuiltinFunc() {
            auto *printFunc = new BuiltinFuncObject();
            printFunc->Value = PrintlnFunc;
            BuiltinFuncMap["println"] = printFunc;
        }

        BuiltinFuncObject *GetBuiltinFunc(string funcName) {
            return BuiltinFuncMap[funcName];
        }
    };
}

#endif
