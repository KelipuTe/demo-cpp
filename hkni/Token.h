#ifndef DEMO_CPP_TOKEN_H
#define DEMO_CPP_TOKEN_H

#include <string>

using namespace std;

//词法标记类型，避开一些常见的关键字
enum TOKEN_TYPE {
    ILLEGAL, //非法
    END, //结束
    COMMENT, //注释

    IDENTIFIER, //标识符

    HKNI_NULL, //空
    HKNI_INT, //整数
    HKNI_FLOAT, //浮点数
    HKNI_STRING, //字符串
    HKNI_ARRAY, //数组

    ASSIGN, //等于号或者赋值（=）

    ADD, //加号（+）
    SUB, //减号（-）
    MUL, //乘号（*）
    DIV, //除号（/）
    MOD, //取模（%）
    INC, //自增（++）
    DEC, //自减（--）

    ADD_ASSIGN, //加赋值（+=）
    SUB_ASSIGN, //减赋值（-=）
    MUL_ASSIGN, //乘赋值（*=）
    DIV_ASSIGN, //除赋值（/=）
    MOD_ASSIGN, //取模赋值（%=）
    ARRAY_ASSIGN, //数组赋值（=>）

    EQ, //等于号（==）
    NEQ, //不等于号（!=）
    GT, //大于号（>）
    GTE, //大于等于号（>=）
    LT, //小于号（<）
    LTE, //小于等于号（<=）

    AND, //逻辑与（&&）
    OR, //逻辑或（||）
    NOT, //逻辑非（!）

    BIT_AND, //位与（&）
    BIT_OR, //位或（|）

    SEMICOLON, //分号（;）
    COLON, //冒号（:）
    COMMA, //逗号（,）
    DOT, //点（.）

    LPAREN, //左括号（（）
    RPAREN, //右括号（））
    LBRACE, //左大括号（{）
    RBRACE, //右大括号（}）
    LBRACKET, //左中括号（[）
    RBRACKET, //右中括号（]）

    //关键字
    HKNI_TRUE, //true
    HKNI_FALSE, //false

    HKNI_VAR, //var 变量定义
    HKNI_CONST, //const 常量定义

    HKNI_IF, //if
    HKNI_ELSE, //else

    HKNI_WHILE, //while
    HKNI_FOR, //for

    HKNI_FUNC, //func 函数定义
    HKNI_RETURN, //return

    HKNI_CLASS, //class 类定义
    HKNI_NEW, //new 类实例化
};

//词法标记
class Token {
public:
    TOKEN_TYPE TokenType; //词法标记类型
    string Literal; //词法标记原始值
};

#endif //DEMO_CPP_TOKEN_H
