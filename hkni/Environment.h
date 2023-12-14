#ifndef HKNI_ENVIRONMENT_H
#define HKNI_ENVIRONMENT_H

#include <map>

#include "object/Object.h"

using namespace object;

namespace env {
    //环境
    class Environment {
        //####属性
    public:
        std::map<string, Object *> variableMap; //局部变量
        Environment *p7outer; //外层环境
        //####方法
    public:
        Environment() {}

        //添加变量，不可重复
        void AddVariable(string name, Object *p7ValueObj) {
            if (!this->IsExistVariable(name)) {
                variableMap[name] = p7ValueObj;
            }
        }

        //获取变量
        Object *GetVariable(string name) {
            if (this->IsExistVariable(name)) {
                return variableMap[name];
            }
            return nullptr;
        }

        //修改变量
        void EditVariable(string name, Object *p7ValueObj) {
            if (this->IsExistVariable(name)) {
                variableMap[name] = p7ValueObj;
            }
        }

        //变量是否存在
        bool IsExistVariable(string name) {
            auto p7obj = variableMap[name];
            return p7obj != nullptr;
        }
    };
}

#endif //HKNI_ENVIRONMENT_H
