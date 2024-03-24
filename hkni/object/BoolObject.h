#ifndef HKNI_BOOL_OBJECT_H
#define HKNI_BOOL_OBJECT_H

#include "Object.h"

namespace objecthkni {
    class BoolObject : public Object {
    public:
        ObjectType ValueObjectType;
        bool Value;

    public:
        BoolObject(bool v) {
            ValueObjectType = BOOL_OBJ;
            Value = v;
        }

        ObjectType GetObjectType() override {
            return ValueObjectType;
        }

        string GetLiteral() override {
            return Value ? "true" : "false";
        }

        string ToString() override {
            string str;
            str.append("Object;ObjectType=" + ObjectTypeToString(ValueObjectType) + ";");
            string valueStr = Value ? "true" : "false";
            str.append("P7Value=" + valueStr + ";");
            return str;
        }
    };
}

#endif
