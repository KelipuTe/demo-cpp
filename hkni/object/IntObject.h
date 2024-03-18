#ifndef HKNI_INT_OBJECT_H
#define HKNI_INT_OBJECT_H

#include "Object.h"

namespace objecthkni {
    class IntObject : public Object {
    public:
        ObjectType ValueObjectType;
        int Value;
    public:
        IntObject(int v) {
            this->ValueObjectType = INT_OBJ;
            this->Value = v;
        }

        ObjectType GetObjectType() override {
            return this->ValueObjectType;
        }

        string GetLiteral() override {
            return to_string(this->Value);
        }

        string ToString() override {
            string t4str;
            t4str.append("Object ObjectType=" + to_string(this->ValueObjectType) + ";");
            t4str.append("Value=" + to_string(this->Value) + ";");
            return t4str;
        }
    };
}

#endif
