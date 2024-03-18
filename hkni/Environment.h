#ifndef HKNI_ENVIRONMENT_H
#define HKNI_ENVIRONMENT_H

#include <map>

#include "object/Object.h"

using namespace objecthkni;

namespace hkni {
    //环境（作用域）
    class Environment {
    public:
        std::map<string, Object *> variableMap; //局部变量
        Environment *p7outer; //外层环境
    public:
        Environment() {}

        //添加变量，不可重复
        void AddVariable(string name, Object *p7ValueObj) {
            if (!IsExistVariable(name)) {
                variableMap[name] = p7ValueObj;
            }
        }

        //获取变量
        Object *GetVariable(string name) {
            if (IsExistVariable(name)) {
                return variableMap[name];
            }
            if (p7outer != nullptr && p7outer->IsExistVariable(name)) {
                return p7outer->GetVariable(name);
            }
            return nullptr;
        }

        //修改变量
        void EditVariable(string name, Object *p7ValueObj) {
            if (IsExistVariable(name)) {
                variableMap[name] = p7ValueObj;
                return;
            }
            if (p7outer != nullptr && p7outer->IsExistVariable(name)) {
                p7outer->variableMap[name] = p7ValueObj;
                return;
            }
        }

        //变量是否存在
        bool IsExistVariable(string name) {
            auto p7obj = variableMap[name];
            return p7obj != nullptr;
        }
    };
}

#endif
