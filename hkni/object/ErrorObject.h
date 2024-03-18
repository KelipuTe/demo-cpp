#ifndef HKNI_ERROR_OBJECT_H
#define HKNI_ERROR_OBJECT_H

#include "Object.h"

namespace objecthkni {
    class ErrorObject : public Object {
        //##属性
    public:
        ObjectType ValueObjectType;
        string Value;
        //##方法
    public:
        ErrorObject(string s) {
            this->ValueObjectType = ERROR_OBJ;
            this->Value = s;
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
