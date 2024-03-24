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
            ValueObjectType = INT_OBJ;
            Value = v;
        }

        ObjectType GetObjectType() override {
            return ValueObjectType;
        }

        string GetLiteral() override {
            return to_string(Value);
        }

        string ToString() override {
            string str;
            str.append("Object;ObjectType=" + ObjectTypeToString(ValueObjectType) + ";");
            str.append("P7Value=" + to_string(Value) + ";");
            return str;
        }
    };
}

#endif
