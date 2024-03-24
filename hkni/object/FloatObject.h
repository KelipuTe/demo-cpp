#ifndef HKNI_FLOAT_OBJECT_H
#define HKNI_FLOAT_OBJECT_H

#include "Object.h"

namespace objecthkni {
    class FloatObject : public Object {
    public:
        ObjectType ValueObjectType;
        float Value;

    public:
        FloatObject(float v) {
            ValueObjectType = FLOAT_OBJ;
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
