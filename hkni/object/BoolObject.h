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
                this->Value = v;
        }

        ObjectType GetObjectType() override {
            return this->ValueObjectType;
        }

        string GetLiteral() override {
            return this->Value ? "true" : "false";
        }

        string ToString() override {
            string str;
            str.append("Object ObjectType=" + ObjectTypeToString(ValueObjectType) + ";");
            string valueStr = this->Value ? "true" : "false";
            str.append("Value=" + valueStr + ";");
            return str;
        }
    };
}

#endif
