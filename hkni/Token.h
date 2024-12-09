#ifndef HKNI_TOKEN_H
#define HKNI_TOKEN_H

#include <string>

using namespace std;

namespace hkni {
    //词法标记类型
    //因为 HKNI 是用 c++ 编写的，所以尽量避开一些 c++ 的关键字
    enum TokenType {
        ILLEGAL, //非法，枚举第一个是0，正好用非法占位
        END, //结束

        //运算符（按键盘从左到右从上到下的顺序排的，方便找）
        STRING_VALUE, //字符串值（`、'、"）
        NOT, //逻辑非（!）
        NEQ, //不等于（!=）
        MOD, //取模（%）
        MOD_ASSIGN, //取模赋值（%=）
        BIT_AND, //位与（&）
        AND, //逻辑与（&&）
        MUL, //乘法（*）
        MUL_ASSIGN, //乘赋值（*=）
        LPAREN, //左括号（(）
        RPAREN, //右括号（)）
        SUB, //减法，负号（-）
        SUB_ASSIGN, //减赋值（-=）
        DEC, //自减（--）
        ASSIGN, //赋值（=）
        EQ, //等于（==）
        ADD, //加法（+）
        ADD_ASSIGN, //加赋值（+=）
        INC, //自增（++）
        LBRACKET, //左中括号（[）
        LBRACE, //左大括号（{）
        RBRACKET, //右中括号（]）
        RBRACE, //右大括号（}）
        BIT_OR, //位或（|）
        OR, //逻辑或（||）
        SEMICOLON, //分号（;）
        COLON, //冒号（:）
        COMMA, //逗号（,）
        LT, //小于（<）
        LTE, //小于等于（<=）
        GT, //大于（>）
        DOT, //点，用于类调用属性和方法（.）
        GTE, //大于等于（>=）
        DIV, //除法（/）
        DIV_ASSIGN, //除赋值（/=）
        COMMENT, //注释（//）

        //关键字
        VAR_HKNI, //变量声明（var）
        CONST_HKNI, //常量声明（const）

        BOOL_TYPE, //布尔类型（bool）
        BOOL_VALUE, //布尔值（true、false）
        INT_TYPE, //整数类型（int）
        INT_VALUE, //整数值（2进制、8进制、10进制、16进制）
        FLOAT_TYPE, //浮点数类型（float）
        FLOAT_VALUE, //浮点数值（10进制）
        STRING_TYPE, //字符串类型（string）
        NULL_HKNI, //null值（null）

        IF_HKNI, //if语句
        ELSE_HKNI, //else语句
        FOR_HKNI, //for循环语句

        FUNCTION_HKNI, //函数定义（func）
        RETURN_HKNI, //函数返回语句（return）

        ARRAY_HKNI, //数组声明（[]）
        MAP_HKNI, //map声明（map）
        TYPE_HKNI, //类型定义（type）
        STRUCT_HKNI, //结构体定义（struct）
        CLASS_HKNI, //类定义（class）
        NEW_HKNI, //实例化（new）

        //其他
        IDENTIFIER, //标识符
    };

    //词法标记
    class Token {
    public:
        string Literal; //词法标记原始值（就是代码文本）
        TokenType TokenType; //词法标记类型
    };

    string TokenTypeToString(TokenType t) {
        switch (t) {
            case BOOL_TYPE:
                return "bool";
            case INT_TYPE:
                return "int";
            case FLOAT_TYPE:
                return "float";
            case STRING_TYPE:
                return "string";
            default:
                return "unknown";
        }
    }
}

#endif
