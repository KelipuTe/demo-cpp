#ifndef HKNI_PRECEDENCE_H
#define HKNI_PRECEDENCE_H

namespace hkni {
    //运算符优先级
    //减号和负号的优先级不一样
    enum PRECEDENCE {
        LOWEST_P, //最低
        ASSIGN_P, // =
        OR_P, // ||
        AND_P, // &&
        BIT_OR_P, // |
        BIT_AND_P, // &
        NEQ_EQ_P, // !=,==
        LT_LTE_GT_GTE_P, // <,<=,>,>=,
        SUB_ADD_P, // -,+
        MOD_MUL_DIV_P, // %,*,/
        NOT_SUB_DEC_INC_P, // !,-,--,++
        LPAREN_P, // (
        // [
        // {
        HIGHEST_P, //最高
    };
}

#endif
