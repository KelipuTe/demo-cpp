#ifndef HKNI_NULL_OBJ_H
#define HKNI_NULL_OBJ_H

#include "Object.h"

namespace hkni {
    class NullObj : public Object {
    public:
        ObjectType ValueObjectType;

    public:
        NullObj() {
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
