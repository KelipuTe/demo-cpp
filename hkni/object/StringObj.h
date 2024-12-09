#ifndef HKNI_STRING_OBJ_H
#define HKNI_STRING_OBJ_H

#include "Object.h"

namespace hkni {
    class StringObj : public Object {
    public:
        ObjectType ValueObjectType;
        string Value;

    public:
        StringObj(string v) {
            ValueObjectType = STRING_OBJ;
            Value = v;
        }

        ObjectType GetObjectType() override {
            return ValueObjectType;
        }

        string GetLiteral() override {
            return Value;
        }

        string ToString() override {
            string str;
            str.append("Object;ObjectType=" + ObjectTypeToString(ValueObjectType) + ";");
            str.append("P7Value=" + Value + ";");
            return str;
        }
    };
}

#endif
