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

        void SetVariable(string name, Object *value) {
            variableMap[name] = value;
        }

        bool IsSetVariable(string name) {
            auto p7obj = variableMap[name];
            return p7obj != nullptr;
        }
    };
}

#endif //HKNI_ENVIRONMENT_H
