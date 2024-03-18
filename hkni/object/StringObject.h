#ifndef HKNI_STRING_OBJECT_H
#define HKNI_STRING_OBJECT_H

#include "Object.h"

namespace objecthkni {
    class StringObject : public Object {
        //##属性
    public:
        ObjectType ValueObjectType;
        string Value;
        //##方法
    public:
        StringObject(string v) {
            this->ValueObjectType = STRING_OBJ;
            this->Value = v;
        }

        ObjectType GetObjectType() override {
            return this->ValueObjectType;
        }

        string GetLiteral() override {
            return this->Value;
        }

        string ToString() override {
            string t4str;
            t4str.append("Object ObjectType=" + to_string(this->ValueObjectType) + ";");
            t4str.append("Value=" + this->Value + ";");
            return t4str;
        }
    };
}

#endif
