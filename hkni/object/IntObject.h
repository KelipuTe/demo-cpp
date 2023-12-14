#ifndef HKNI_INT_OBJECT_H
#define HKNI_INT_OBJECT_H

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
            this->Type = INT_OBJ;
            this->Value = value;
        }

        OBJECT_TYPE GetType() override {
            return this->Type;
        }

        string GetLiteral() override {
            return to_string(this->Value);
        }

        string ToString() override {
            string t4str;
            t4str.append("Object Type=" + to_string(this->Type) + ";");
            t4str.append("Value=" + to_string(this->Value) + ";");
            return t4str;
        }
    };
}

#endif //HKNI_INT_OBJECT_H
