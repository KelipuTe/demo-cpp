#ifndef HKNI_PRECEDENCE_H
#define HKNI_PRECEDENCE_H

// 运算符优先级
enum PRECEDENCE{
    LOWEST_P, //最低
    ASSIGN_P, //=
    //||
    //&&
    //|
    //&
    //==,!=
    //>,<,>=,<=
    ADD_SUB_P, //+,-
    MUL_DIV_MOD_P, //*,/,%
    //-,++,--,!
    LPAREN_P, //(
    //[
    //{
    HIGHEST_P, //最高
};

#endif //HKNI_PRECEDENCE_H
