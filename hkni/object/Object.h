#ifndef HKNI_OBJECT_H
#define HKNI_OBJECT_H

#include <string>

using namespace std;

namespace object {
    //对象类型，避开一些常见的关键字
    enum OBJECT_TYPE {
        BOOL_OBJ, //布尔值
        INT_OBJ, //整数
        FLOAT_OBJ, //浮点数
        STRING_OBJ, //字符串
        ERROR, //错误
    };

    class Object {
    public:
        Object() = default;

        virtual ~Object() = default;

        //获取对象类型
        virtual OBJECT_TYPE GetType() = 0;

        //获取对象原始值
        virtual string GetLiteral() = 0;

        virtual string ToString() = 0;
    };
}

#endif //HKNI_OBJECT_H
