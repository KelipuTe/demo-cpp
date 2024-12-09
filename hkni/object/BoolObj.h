#ifndef HKNI_BOOL_OBJ_H
#define HKNI_BOOL_OBJ_H

#include "Object.h"

namespace hkni {
    class BoolObj : public Object {
    public:
        ObjectType ValueObjectType;
        bool Value;

    public:
        BoolObj(bool v) {
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
            str.append("Object;ObjectType=" + ObjectTypeToStr(ValueObjectType) + ";");
            string valueStr = Value ? "true" : "false";
            str.append("P7Value=" + valueStr + ";");
            return str;
        }
    };
}

#endif
