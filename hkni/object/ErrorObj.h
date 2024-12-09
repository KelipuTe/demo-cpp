#ifndef HKNI_ERROR_OBJ_H
#define HKNI_ERROR_OBJ_H

#include "Object.h"

namespace hkni {
    class ErrorObj : public Object {
    public:
        ObjectType ValueObjectType;
        string Value;

    public:
        ErrorObj(string s) {
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
            str.append("Object;ObjectType=" + ObjectTypeToStr(ValueObjectType) + ";");
            str.append("P7Value=" + Value + ";");
            return str;
        }
    };
}

#endif
