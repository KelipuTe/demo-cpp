#ifndef HKNI_RETURN_OBJECT_H
#define HKNI_RETURN_OBJECT_H

#include "Object.h"

namespace objecthkni {
    class ReturnObject : public Object {
        //##属性
    public:
        ObjectType ValueObjectType;
        Object * Value;
        //##方法
    public:
        ReturnObject(Object *p7v) {
            this->ValueObjectType = RETURN_OBJ;
            this->Value = p7v;
        }

        ObjectType GetObjectType() override {
            return this->ValueObjectType;
        }

        string GetLiteral() override {
            return this->Value->GetLiteral();
        }

        string ToString() override {
            string t4str;
            t4str.append("Object ObjectType=" + to_string(this->ValueObjectType) + ";");
            string valueStr = this->Value->ToString();
            t4str.append("Value=" + valueStr + ";");
            return t4str;
        }
    };
}

#endif
