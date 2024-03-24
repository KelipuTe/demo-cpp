#ifndef HKNI_STRING_OBJECT_H
#define HKNI_STRING_OBJECT_H

#include "Object.h"

namespace objecthkni {
    class StringObject : public Object {
    public:
        ObjectType ValueObjectType;
        string Value;

    public:
        StringObject(string v) {
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
