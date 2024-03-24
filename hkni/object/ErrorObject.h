#ifndef HKNI_ERROR_OBJECT_H
#define HKNI_ERROR_OBJECT_H

#include "Object.h"

namespace objecthkni {
    class ErrorObject : public Object {
    public:
        ObjectType ValueObjectType;
        string Value;

    public:
        ErrorObject(string s) {
            ValueObjectType = ERROR_OBJ;
            Value = s;
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
