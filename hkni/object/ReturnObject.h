#ifndef HKNI_RETURN_OBJECT_H
#define HKNI_RETURN_OBJECT_H

#include "Object.h"

namespace objecthkni {
    class ReturnObject : public Object {
    public:
        ObjectType ValueObjectType;
        Object *P7Value;

    public:
        ReturnObject(Object *p7value) {
            ValueObjectType = RETURN_OBJ;
            P7Value = p7value;
        }

        ObjectType GetObjectType() override {
            return ValueObjectType;
        }

        string GetLiteral() override {
            return P7Value->GetLiteral();
        }

        string ToString() override {
            string str;
            str.append("Object;ObjectType=" + ObjectTypeToString(ValueObjectType) + ";");
            str.append("P7Value=" + P7Value->ToString() + ";");
            return str;
        }
    };
}

#endif
