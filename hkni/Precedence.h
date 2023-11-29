#ifndef HKNI_PRECEDENCE_H
#define HKNI_PRECEDENCE_H

// 运算符优先级
// 减号和负号的优先级不一样
enum PRECEDENCE{
    LOWEST_P, //最低
    ASSIGN_P, //=
    OR_P, //||
    AND_P, //&&
    BIT_OR_P, //|
    BIT_AND_P, //&
    EQ_NEQ_P, //==,!=
    GT_LT_GTE_LTE_P, //>,<,>=,<=
    ADD_SUB_P, //+,-
    MUL_DIV_MOD_P, //*,/,%
    SUB_INC_DEC_NOT_P, //-,++,--,!
    LPAREN_P, //(
    //[
    //{
    HIGHEST_P, //最高
};

#endif //HKNI_PRECEDENCE_H
