#ifndef HKNI_BUILTIN_FUNC_H
#define HKNI_BUILTIN_FUNC_H

#include <functional>
#include <map>

#include "../object/BuiltinFuncObj.h"
#include "PrintlnFunc.h"

namespace hkni {
    //内置函数
    class BuiltinFunc {
        //##属性
    public:
        //内置函数map，key为函数名，value为函数对象
        std::map<string, BuiltinFuncObj *> BuiltinFuncMap;

        //##方法
    public:
        BuiltinFunc() {
            InitBuiltinFunc();
        }

        void InitBuiltinFunc() {
            auto *printFunc = new BuiltinFuncObj();
            printFunc->Value = PrintlnFunc;
            BuiltinFuncMap["println"] = printFunc;
        }

        BuiltinFuncObj *GetBuiltinFunc(string funcName) {
            return BuiltinFuncMap[funcName];
        }
    };
}

#endif
