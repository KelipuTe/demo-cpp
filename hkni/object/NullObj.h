#ifndef HKNI_NULL_OBJECT_H
#define HKNI_NULL_OBJECT_H

#include "Object.h"

namespace object {
    class NullObject : public Object {
    public:
        OBJECT_TYPE Type;
    public:
        NullObject() {
            this->Type = NULL_OBJ;
        }

        OBJECT_TYPE GetType() override {
            return this->Type;
        }

        string GetLiteral() override {
            return "NULL";
        }

        string ToString() override {
            string t4str;
            t4str.append("Object Type=" + to_string(this->Type) + ";");
            return t4str;
        }
    };
}

#endif //HKNI_NULL_OBJECT_H
