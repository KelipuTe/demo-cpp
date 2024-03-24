#ifndef HKNI_ENVIRONMENT_H
#define HKNI_ENVIRONMENT_H

#include <map>

#include "object/Object.h"

using namespace objecthkni;

namespace hkni {
    //环境（作用域）
    class Environment {
    public:
        std::map<string, Object *> VariableMap; //局部变量
        Environment *P7OuterEnv; //外层环境
    public:
        Environment() {}

        //添加变量，不可重复
        void AddVariable(string name, Object *p7ValueObj) {
            if (!IsExistVariable(name)) {
                VariableMap[name] = p7ValueObj;
            }
        }

        //获取变量
        Object *GetVariable(string name) {
            if (IsExistVariable(name)) {
                return VariableMap[name];
            }
            if (P7OuterEnv != nullptr && P7OuterEnv->IsExistVariable(name)) {
                return P7OuterEnv->GetVariable(name);
            }
            return nullptr;
        }

        //修改变量
        void EditVariable(string name, Object *p7ValueObj) {
            if (IsExistVariable(name)) {
                VariableMap[name] = p7ValueObj;
                return;
            }
            if (P7OuterEnv != nullptr) {
                P7OuterEnv->EditVariable(name, p7ValueObj);
                return;
            }
        }

        //变量是否存在
        bool IsExistVariable(string name) {
            auto p7obj = VariableMap[name];
            return p7obj != nullptr;
        }
    };
}

#endif
