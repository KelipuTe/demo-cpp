#ifndef HKNI_RETURN_OBJECT_H
#define HKNI_RETURN_OBJECT_H

#include "Object.h"

namespace object {
    class ReturnObject : public Object {
        //####属性
    public:
        OBJECT_TYPE Type;
        Object * Value;
        //####方法
    public:
        ReturnObject(Object *p7obj) {
            this->Type = RETURN_OBJ;
            this->Value = p7obj;
        }

        OBJECT_TYPE GetType() override {
            return this->Type;
        }

        string GetLiteral() override {
            return this->Value->GetLiteral();
        }

        string ToString() override {
            string t4str;
            t4str.append("Object Type=" + to_string(this->Type) + ";");
            string valueStr = this->Value->ToString();
            t4str.append("Value=" + valueStr + ";");
            return t4str;
        }
    };
}

#endif //HKNI_RETURN_OBJECT_H
