//这个文件放解释数据类型（布尔、浮点型、整型、字符串）的代码

#include "../Interpreter.h"

namespace hkni {
    Object *Interpreter::itptBoolInfixExp(Object *left, string op, Object *right) {
        auto *leftReal = dynamic_cast<BoolObj *>(left);
        auto *rightReal = dynamic_cast<BoolObj *>(right);

        if (op == "&&") {
            return new BoolObj(leftReal->Value && rightReal->Value);
        }
        if (op == "||") {
            return new BoolObj(leftReal->Value || rightReal->Value);
        }

        return new ErrorObj("错误。整数布尔不支持中缀操作符：" + op);
    }

    Object *Interpreter::itptBoolPrefixExp(string op, Object *obj) {
        auto *objReal = dynamic_cast<BoolObj *>(obj);

        if (op == "!") {
            return new BoolObj(!objReal->Value);
        }

        return new ErrorObj("错误。布尔类型不支持前缀操作符：" + op);
    }

    Object *Interpreter::itptFloatInfixExp(Object *left, string op, Object *right) {
        auto *leftReal = dynamic_cast<FloatObj *>(left);
        auto *rightReal = dynamic_cast<FloatObj *>(right);

        //c++里面可以用运算符比较浮点数，但是这里用常规的相减之后和0比的办法。
        if (op == "+") {
            return new FloatObj(leftReal->Value + rightReal->Value);
        }
        if (op == "-") {
            return new FloatObj(leftReal->Value - rightReal->Value);
        }
        if (op == "*") {
            return new FloatObj(leftReal->Value * rightReal->Value);
        }
        if (op == "/") {
            return new FloatObj(leftReal->Value / rightReal->Value);
        }
        if (op == "==") {
            return new BoolObj((leftReal->Value - rightReal->Value) < 0.000001);
        }
        if (op == "!=") {
            return new BoolObj((leftReal->Value - rightReal->Value) > 0.000001);
        }
        if (op == ">") {
            return new BoolObj((leftReal->Value - rightReal->Value) > 0);
        }
        if (op == "<") {
            return new BoolObj((leftReal->Value - rightReal->Value) < 0);
        }

        return new ErrorObj("错误。浮点数类型不支持中缀操作符：" + op);
    }

    Object *Interpreter::itptFloatPrefixExp(string op, Object *obj) {
        auto *objReal = dynamic_cast<FloatObj *>(obj);

        if (op == "-") {
            return new FloatObj(-objReal->Value);
        }
        if (op == "++") {
            return new FloatObj(++objReal->Value); //执行操作后，变量的值会变
        }
        if (op == "--") {
            return new FloatObj(--objReal->Value); //执行操作后，变量的值会变
        }

        return new ErrorObj("错误。浮点数类型不支持前缀操作符：" + op);
    }

    Object *Interpreter::itptIntInfixExp(Object *left, string op, Object *right) {
        auto *leftReal = dynamic_cast<IntObj *>(left);
        auto *rightReal = dynamic_cast<IntObj *>(right);

        if (op == "+") {
            return new IntObj(leftReal->Value + rightReal->Value);
        }
        if (op == "-") {
            return new IntObj(leftReal->Value - rightReal->Value);
        }
        if (op == "*") {
            return new IntObj(leftReal->Value * rightReal->Value);
        }
        if (op == "/") {
            return new IntObj(leftReal->Value / rightReal->Value);
        }
        if (op == "%") {
            return new IntObj(leftReal->Value % rightReal->Value);
        }
        if (op == "==") {
            return new BoolObj(leftReal->Value == rightReal->Value);
        }
        if (op == "!=") {
            return new BoolObj(leftReal->Value != rightReal->Value);
        }
        if (op == ">") {
            return new BoolObj(leftReal->Value > rightReal->Value);
        }
        if (op == "<") {
            return new BoolObj(leftReal->Value < rightReal->Value);
        }
        if (op == ">=") {
            return new BoolObj(leftReal->Value >= rightReal->Value);
        }
        if (op == "<=") {
            return new BoolObj(leftReal->Value <= rightReal->Value);
        }
        if (op == "&") {
            return new IntObj(leftReal->Value & rightReal->Value);
        }
        if (op == "|") {
            return new IntObj(leftReal->Value | rightReal->Value);
        }

        return new ErrorObj("错误。整数类型不支持中缀操作符：" + op);
    }

    Object *Interpreter::itptIntPrefixExp(string op, Object *obj) {
        auto *objReal = dynamic_cast<IntObj *>(obj);

        if (op == "-") {
            return new IntObj(-objReal->Value);
        }
        if (op == "++") {
            return new IntObj(++objReal->Value); //执行操作后，变量的值会变
        }
        if (op == "--") {
            return new IntObj(--objReal->Value); //执行操作后，变量的值会变
        }

        return new ErrorObj("错误。整数类型不支持前缀操作符：" + op);
    }
}
