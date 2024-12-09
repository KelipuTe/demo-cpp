#ifndef HKNI_NEW_EXP_H
#define HKNI_NEW_EXP_H

#include <string>
#include "../Token.h"
#include "../ast/Expression.h"

namespace hkni {
    //实例化达式
    class NewExp : public ExpressionI9 {
    public:
        Token TokenHKNI; //就是操作符的字符串
        ExpressionI9 *ExpI9; //值表达式，需要先解释出来
    public:
        NewExp(Token t) {
            this->TokenHKNI = t;
            this->ExpI9 = nullptr;
        }

        string GetTokenLiteral() override {
            return this->TokenHKNI.Literal;
        }

        string ToString() override {
            string t4str;
            t4str.append("(new ");
            t4str.append(this->ExpI9->ToString());
            t4str.append(")");
            return t4str;
        }

        void ExpressionNode() override {}
    };
}

#endif
