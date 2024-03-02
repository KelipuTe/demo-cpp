#ifndef HKNI_OBJECT_H
#define HKNI_OBJECT_H

#include <string>

using namespace std;

namespace object {
    //对象类型，避开一些常见的关键字
    enum OBJECT_TYPE {
        ERROR_OBJ, //错误
        NULL_OBJ, //空值
        BOOL_OBJ, //布尔值
        INT_OBJ, //整数
        FLOAT_OBJ, //浮点数
        STRING_OBJ, //字符串
        FUNC_OBJ, //函数
        BUILTIN_FUNC_OBJ, //函数
        RETURN_OBJ, //返回值
    };

    class Object {
    protected:
        bool isConst = false;
    public:
        Object() = default;

        //获取对象类型
        virtual OBJECT_TYPE GetType() = 0;

        //获取对象原始值
        virtual string GetLiteral() = 0;

        virtual string ToString() = 0;
    };
}

#endif //HKNI_OBJECT_H
