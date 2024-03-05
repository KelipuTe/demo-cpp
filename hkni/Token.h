#ifndef HKNI_TOKEN_H
#define HKNI_TOKEN_H

#include <string>

using namespace std;

//词法标记类型，避开一些常见的关键字
enum TokenType {
    ILLEGAL, //非法
    END, //结束

    COMMENT, //注释

    NULL_HKNI, //null值（null）

    BOOL_TYPE, //布尔类型（bool）
    TRUE_VALUE, //布尔真值（true）
    FALSE_VALUE, //布尔假值（false）

    INT_TYPE, //整数类型（int）
    INT_VALUE, //整数值

    FLOAT_TYPE, //浮点数类型（float）
    FLOAT_VALUE, //浮点数值

    STRING_TYPE, //字符串类型（string）
    STRING_VALUE, //字符串值

    ARRAY_TYPE, //数组类型（array）
    ARRAY_VALUE, //数组值
    ARRAY_ASSIGN, //数组赋值（=>）

    ADD, //加号（+）
    SUB, //减号，负号（-）
    MUL, //乘号（*）
    DIV, //除号（/）
    MOD, //取模（%）

    EQ, //等于号（==）
    NEQ, //不等于号（!=）
    GT, //大于号（>）
    LT, //小于号（<）
    GTE, //大于等于号（>=）
    LTE, //小于等于号（<=）

    AND, //逻辑与（&&）
    OR, //逻辑或（||）
    NOT, //逻辑非（!）

    BIT_AND, //位与（&）
    BIT_OR, //位或（|）

    IDENTIFIER, //标识符

    VAR_HKNI, //变量声明（var）
    CONST_HKNI, //常量声明（const）

    ASSIGN, //赋值（=）
    ADD_ASSIGN, //加赋值（+=）
    SUB_ASSIGN, //减赋值（-=）
    MUL_ASSIGN, //乘赋值（*=）
    DIV_ASSIGN, //除赋值（/=）
    MOD_ASSIGN, //取模赋值（%=）

    INC, //自增（++）
    DEC, //自减（--）

    SEMICOLON, //分号（;）
    COLON, //冒号（:）
    COMMA, //逗号（,）
    DOT, //点，用于类调用属性和方法（.）

    LPAREN, //左括号（(）
    RPAREN, //右括号（)）
    LBRACKET, //左中括号（[）
    RBRACKET, //右中括号（]）
    LBRACE, //左大括号（{）
    RBRACE, //右大括号（}）

    IF_HKNI, //if语句
    ELSE_HKNI, //else语句

    FOR_HKNI, //for循环语句

    FUNCTION_HKNI, //函数定义（function）
    RETURN_HKNI, //返回语句（return）

    STRUCT_HKNI, //结构体定义（struct）
    NEW_HKNI, //结构体实例化（new）
};

//词法标记
class Token {
public:
    TokenType TokenType; //词法标记类型
    string Literal; //词法标记原始值
};

#endif
