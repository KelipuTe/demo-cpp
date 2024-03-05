#ifndef HKNI_BOOL_OBJECT_H
#define HKNI_BOOL_OBJECT_H

#include "Object.h"

namespace object {
    class BoolObject : public Object {
        //##属性
    public:
        OBJECT_TYPE Type;
        bool Value;
        //##方法
    public:
        BoolObject(int value) {
            this->Type = BOOL_OBJ;
            this->Value = value;
        }

        OBJECT_TYPE GetType() override {
            return this->Type;
        }

        string GetLiteral() override {
            return this->Value ? "true" : "false";
        }

        string ToString() override {
            string t4str;
            t4str.append("Object Type=" + to_string(this->Type) + ";");
            string valueStr = this->Value ? "true" : "false";
            t4str.append("Value=" + valueStr + ";");
            return t4str;
        }
    };
}

#endif
