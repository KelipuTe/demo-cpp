#ifndef HKNI_STRING_OBJECT_H
#define HKNI_STRING_OBJECT_H

#include "Object.h"

namespace object {
    class StringObject : public Object {
        //##属性
    public:
        OBJECT_TYPE Type;
        string Value;
        //##方法
    public:
        StringObject(string value) {
            this->Type = STRING_OBJ;
            this->Value = value;
        }

        OBJECT_TYPE GetType() override {
            return this->Type;
        }

        string GetLiteral() override {
            return this->Value;
        }

        string ToString() override {
            string t4str;
            t4str.append("Object Type=" + to_string(this->Type) + ";");
            t4str.append("Value=" + this->Value + ";");
            return t4str;
        }
    };
}

#endif
