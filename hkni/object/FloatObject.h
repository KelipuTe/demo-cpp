#ifndef HKNI_FLOAT_OBJECT_H
#define HKNI_FLOAT_OBJECT_H

#include "Object.h"

namespace object {
    class FloatObject : public Object {
        //####属性
    public:
        OBJECT_TYPE Type;
        float Value;
        //####方法
    public:
        FloatObject(float value) {
            this->Type = FLOAT_HKNI;
            this->Value = value;
        }

        OBJECT_TYPE GetType() override {
            return this->Type;
        }

        string GetValue() override {
            return to_string(this->Value);
        }

        string ToString() override {
            string t4str;
            t4str.append("Type=" + to_string(this->Type) + ";");
            t4str.append("Value=" + to_string(this->Value) + ";");
            return t4str;
        }
    };
}

#endif //HKNI_FLOAT_OBJECT_H
