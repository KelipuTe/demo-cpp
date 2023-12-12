#ifndef HKNI_OBJECT_H
#define HKNI_OBJECT_H

#include <string>

using namespace std;

namespace object {
    //对象类型，避开一些常见的关键字
    enum OBJECT_TYPE {
        INT_HKNI, //整数
    };

    class Object {
    public:
        Object() {}

        virtual ~Object() {}

        virtual OBJECT_TYPE GetType() = 0;

        virtual string GetValue() = 0;
    };
}

#endif //HKNI_OBJECT_H
