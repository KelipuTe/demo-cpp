#ifndef HKNI_INT_OBJECT_H
#define HKNI_INT_OBJECT_H

#include <string>

#include "Object.h"

namespace object {
    class IntObject : public Object {
        //####属性
    public:
        OBJECT_TYPE Type;
        int Value;
        //####方法
    public:
        IntObject(int value) {
            this->Type=INT_HKNI;
            this->Value = value;
        }

        OBJECT_TYPE GetType() override {
            return this->Type;
        }

        string GetValue() override {
            return to_string(this->Value);
        }
    };
}

#endif //HKNI_INT_OBJECT_H
