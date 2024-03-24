#ifndef HKNI_OBJECT_H
#define HKNI_OBJECT_H

#include <string>

using namespace std;

namespace objecthkni {
    //对象类型，避开一些常见的关键字
    enum ObjectType {
        ERROR_OBJ, //错误
        NULL_OBJ, //空值
        BOOL_OBJ, //布尔
        INT_OBJ, //整数
        FLOAT_OBJ, //浮点数
        STRING_OBJ, //字符串
        FUNC_OBJ, //函数
        BUILTIN_FUNC_OBJ, //内置函数
        RETURN_OBJ, //返回
    };

    string ObjectTypeToString(ObjectType ot) {
        switch (ot) {
            case ERROR_OBJ:
                return "ERROR_OBJ";
            case NULL_OBJ:
                return "NULL_OBJ";
            case BOOL_OBJ:
                return "BOOL_OBJ";
            case INT_OBJ:
                return "INT_OBJ";
            case FLOAT_OBJ:
                return "FLOAT_OBJ";
            case STRING_OBJ:
                return "STRING_OBJ";
            case FUNC_OBJ:
                return "FUNC_OBJ";
            case BUILTIN_FUNC_OBJ:
                return "BUILTIN_FUNC_OBJ";
            case RETURN_OBJ:
                return "RETURN_OBJ";
            default:
                return "UNKNOWN_OBJ";
        }
    }

    //对象抽象
    class Object {
    public:
        bool IsConst = false; //是否是常量

    public:
        Object() = default;

        //获取对象类型
        virtual ObjectType GetObjectType() = 0;

        //获取对象原始值
        virtual string GetLiteral() = 0;

        virtual string ToString() = 0;
    };

    //判断对象是不是某种类型
    bool ObjectIs(Object *p7o, ObjectType ot) {
        if (p7o == nullptr) {
            return false;
        }
        return p7o->GetObjectType() == ot;
    }
}

#endif
