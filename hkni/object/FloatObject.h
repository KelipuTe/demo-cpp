#ifndef HKNI_FLOAT_OBJECT_H
#define HKNI_FLOAT_OBJECT_H

#include "Object.h"

namespace objecthkni {
    class FloatObject : public Object {
        //##属性
    public:
        ObjectType ValueObjectType;
        float Value;
        //##方法
    public:
        FloatObject(float v) {
            this->ValueObjectType = FLOAT_OBJ;
            this->Value = v;
        }

        ObjectType GetObjectType() override {
            return this->ValueObjectType;
        }

        string GetLiteral() override {
            return to_string(this->Value);
        }

        string ToString() override {
            string t4str;
            t4str.append("Object ObjectType=" + to_string(this->ValueObjectType) + ";");
            t4str.append("Value=" + to_string(this->Value) + ";");
            return t4str;
        }
    };
}

#endif
