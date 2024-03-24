#ifndef HKNI_NULL_OBJECT_H
#define HKNI_NULL_OBJECT_H

#include "Object.h"

namespace objecthkni {
    class NullObject : public Object {
    public:
        ObjectType ValueObjectType;

    public:
        NullObject() {
            this->ValueObjectType = NULL_OBJ;
        }

        ObjectType GetObjectType() override {
            return this->ValueObjectType;
        }

        string GetLiteral() override {
            return "NULL";
        }

        string ToString() override {
            return "Object;ObjectType=" + ObjectTypeToString(ValueObjectType) + ";";
        }
    };
}

#endif
