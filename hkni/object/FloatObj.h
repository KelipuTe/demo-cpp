#ifndef HKNI_FLOAT_OBJ_H
#define HKNI_FLOAT_OBJ_H

#include "Object.h"

namespace hkni {
    class FloatObj : public Object {
    public:
        ObjectType ValueObjectType;
        float Value;

    public:
        FloatObj(float v) {
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
