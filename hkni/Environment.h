#ifndef HKNI_ENVIRONMENT_H
#define HKNI_ENVIRONMENT_H

#include <map>

#include "object/Object.h"

namespace hkni {
    //环境（作用域）
    class Environment {
    public:
        std::map<string, Object *> VarMap; //局部变量
        Environment *outerEnv; //外层环境

    public:
        Environment() {
        }

        //添加变量，不可重复
        void AddVar(string name, Object *obj) {
            if (!IsVarExist(name)) {
                VarMap[name] = obj;
            }
        }

        //获取变量
        Object *GetVar(string name) {
            if (IsVarExist(name)) {
                return VarMap[name];
            }

            if (outerEnv != nullptr && outerEnv->IsVarExist(name)) {
                return outerEnv->GetVar(name);
            }

            return nullptr;
        }

        //修改变量
        void EditVar(string name, Object *obj) {
            if (IsVarExist(name)) {
                VarMap[name] = obj;
                return;
            }

            if (outerEnv != nullptr) {
                outerEnv->EditVar(name, obj);
                return;
            }
        }

        //变量是否存在
        bool IsVarExist(string name) {
            auto obj = VarMap[name];

            return obj != nullptr;
        }
    };
}

#endif
