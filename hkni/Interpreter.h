#ifndef HKNI_INTERPRETER_H
#define HKNI_INTERPRETER_H

#include <typeinfo>

#include "ast/Program.h"

#include "Environment.h"

#include "object/IntObject.h"
#include "object/FloatObject.h"
#include "object/StringObject.h"
#include "object/BoolObject.h"
#include "object/ErrorObject.h"

using namespace ast;
using namespace env;
using namespace object;

class Interpreter {
    //####方法
public:
    //解释器入口，递归入口
    Object *DoInterpret(I9Node *i9node, Environment *p7env) {
        if (typeid(*i9node) == typeid(Program)) {
            auto *p7program = dynamic_cast<Program *>(i9node); //程序
            return interpretProgram(p7program, p7env);
        } else if (typeid(*i9node) == typeid(VarStatement)) {
            auto *p7stmt = dynamic_cast<VarStatement *>(i9node); //变量声明语句
            return interpretVarStatement(p7stmt, p7env);
        } else if (typeid(*i9node) == typeid(ExpressionStatement)) {
            auto *p7stmt = dynamic_cast<ExpressionStatement *>(i9node); //表达式语句
            return DoInterpret(p7stmt->I9Exp, p7env);
        } else if (typeid(*i9node) == typeid(PrefixExpression)) {
            auto *p7exp = dynamic_cast<PrefixExpression *>(i9node); //前缀表达式
            return interpretPrefixExpression(p7exp, p7env);
        } else if (typeid(*i9node) == typeid(InfixExpression)) {
            auto *p7exp = dynamic_cast<InfixExpression *>(i9node); //中缀表达式
            return interpretInfixExpression(p7exp, p7env);
        } else if (typeid(*i9node) == typeid(IdentifierExpression)) {
            auto *p7exp = dynamic_cast<IdentifierExpression *>(i9node); //标识符表达式
            return interpretIdentifierExpression(p7exp, p7env);
        } else if (typeid(*i9node) == typeid(BoolExpression)) {
            auto *p7exp = dynamic_cast<BoolExpression *>(i9node); //布尔表达式
            return new BoolObject(p7exp->GetTokenType() == TRUE_VALUE);
        } else if (typeid(*i9node) == typeid(IntExpression)) {
            auto *p7exp = dynamic_cast<IntExpression *>(i9node); //整数表达式
            return new IntObject(p7exp->GetValue());
        } else if (typeid(*i9node) == typeid(FloatExpression)) {
            auto *p7exp = dynamic_cast<FloatExpression *>(i9node); //浮点数表达式
            return new FloatObject(p7exp->GetValue());
        } else if (typeid(*i9node) == typeid(StringExpression)) {
            auto *p7exp = dynamic_cast<StringExpression *>(i9node); //字符串表达式
            return new StringObject(p7exp->GetTokenLiteral());
        } else if (typeid(*i9node) == typeid(AssignExpression)) {
            auto *p7exp = dynamic_cast<AssignExpression *>(i9node); //赋值表达式
            return interpretAssignExpression(p7exp, p7env);
        }
        return nullptr;
    }

private:
    //判断对象是不是某种类型
    bool objectIs(Object *p7obj, OBJECT_TYPE type) {
        return p7obj->GetType() == type;
    }

    //解释程序
    Object *interpretProgram(Program *p7program, Environment *p7env) {
        Object *p7obj = nullptr;
        for (auto i9stmt: p7program->I9StatementList) {
            p7obj = this->DoInterpret(i9stmt, p7env); //解释每一条语句
        }
        return p7obj;
    }

    //解释变量声明语句
    Object *interpretVarStatement(VarStatement *p7stmt, Environment *p7env) {
        string name = p7stmt->P7NameExp->GetTokenLiteral(); //变量名直接可以拿到
        Object *p7ValueObj = nullptr;
        if (p7stmt->I9ValueExp != nullptr) {
            p7ValueObj = this->DoInterpret(p7stmt->I9ValueExp, p7env); //解释值表达式
        } else {
            //初始化0值
            if (p7stmt->tokenType == BOOL_TYPE) {
                p7ValueObj = new BoolObject(false);
            } else if (p7stmt->tokenType == INT_TYPE) {
                p7ValueObj = new IntObject(0);
            } else if (p7stmt->tokenType == FLOAT_TYPE) {
                p7ValueObj = new FloatObject(0.0);
            } else if (p7stmt->tokenType == STRING_TYPE) {
                p7ValueObj = new StringObject("");
            }
        }
        p7env->AddVariable(name, p7ValueObj); //在环境中添加变量
        return nullptr;
    }

    //解释前缀表达式
    Object *interpretPrefixExpression(PrefixExpression *p7exp, Environment *p7env) {
        auto p7RightObj = this->DoInterpret(p7exp->I9RightExp, p7env);

        Object *p7obj = nullptr;

        //先判断前缀表达式的右边是什么类型，然后根据操作符进行相应的运算
        if (objectIs(p7RightObj, object::INT_OBJ)) {
            p7obj = this->interpretIntPrefixExpression(p7exp->OperatorStr, p7RightObj);
        }

        return p7obj;
    }

    //解释整数前缀表达式
    Object *interpretIntPrefixExpression(string operatorStr, Object *p7obj) {
        IntObject *p7IntObj = dynamic_cast<IntObject *>(p7obj);

        if (operatorStr == "-") {
            return new IntObject(-p7IntObj->Value);
        } else if (operatorStr == "++") {
            return new IntObject(++p7IntObj->Value); //执行操作后，变量的值会变
        } else if (operatorStr == "--") {
            return new IntObject(--p7IntObj->Value); //执行操作后，变量的值会变
        } else {
            return new ErrorObject("错误的前缀操作符：" + operatorStr);
        }
    }

    //解释中缀表达式
    Object *interpretInfixExpression(InfixExpression *p7exp, Environment *p7env) {
        auto p7LeftObj = this->DoInterpret(p7exp->I9LeftExp, p7env);
        auto p7RightObj = this->DoInterpret(p7exp->I9RightExp, p7env);

        return this->handelInfixExpression(p7LeftObj, p7exp->OperatorStr, p7RightObj);
    }

    //处理中缀表达式
    Object *handelInfixExpression(Object *p7LeftObj, string operatorStr, Object *p7RightObj) {
        Object *p7obj = nullptr;

        if (p7LeftObj->GetType() != p7RightObj->GetType()) {
            return new ErrorObject("对象类型不一样");
        }

        if (objectIs(p7LeftObj, object::INT_OBJ)) {
            p7obj = this->interpretIntInfixExpression(p7LeftObj, operatorStr, p7RightObj);
        }

        return p7obj;
    }

    //解释整数中缀表达式
    Object *interpretIntInfixExpression(Object *p7LeftObj, string operatorStr, Object *p7RightObj) {
        IntObject *p7LeftIntObj = dynamic_cast<IntObject *>(p7LeftObj);
        IntObject *p7RightIntObj = dynamic_cast<IntObject *>(p7RightObj);

        if (operatorStr == "+") {
            return new IntObject(p7LeftIntObj->Value + p7RightIntObj->Value);
        } else if (operatorStr == "-") {
            return new IntObject(p7LeftIntObj->Value - p7RightIntObj->Value);
        } else if (operatorStr == "*") {
            return new IntObject(p7LeftIntObj->Value * p7RightIntObj->Value);
        } else if (operatorStr == "/") {
            return new IntObject(p7LeftIntObj->Value / p7RightIntObj->Value);
        } else if (operatorStr == "%") {
            return new IntObject(p7LeftIntObj->Value % p7RightIntObj->Value);
        } else if (operatorStr == "==") {
            return new BoolObject(p7LeftIntObj->Value == p7RightIntObj->Value);
        } else if (operatorStr == "!=") {
            return new BoolObject(p7LeftIntObj->Value != p7RightIntObj->Value);
        } else if (operatorStr == ">") {
            return new BoolObject(p7LeftIntObj->Value > p7RightIntObj->Value);
        } else if (operatorStr == "<") {
            return new BoolObject(p7LeftIntObj->Value < p7RightIntObj->Value);
        } else if (operatorStr == ">=") {
            return new BoolObject(p7LeftIntObj->Value >= p7RightIntObj->Value);
        } else if (operatorStr == "<=") {
            return new BoolObject(p7LeftIntObj->Value <= p7RightIntObj->Value);
        } else {
            return new ErrorObject("错误的中缀操作符：" + operatorStr);
        }
    }

    //解释标识符表达式
    Object *interpretIdentifierExpression(IdentifierExpression *p7exp, Environment *p7env) {
        string name = p7exp->GetTokenLiteral(); //拿到变量名
        auto p7obj = p7env->GetVariable(name); //去环境中获取变量
        if (p7obj != nullptr) {
            return p7obj;
        }
        return new ErrorObject("找不到变量：" + name);
    }

    //解释赋值表达式
    Object *interpretAssignExpression(AssignExpression *p7exp, Environment *p7env) {
        string name = p7exp->I9NameExp->GetTokenLiteral(); //拿到变量名
        auto p7NameObj = this->DoInterpret(p7exp->I9NameExp, p7env); //先从环境中获取变量
        string operatorStr = p7exp->operationStr; //拿到操作符
        auto p7ValueObj = this->DoInterpret(p7exp->I9ValueExp, p7env); //解释变量值

        //运算并赋值的这几个，都需要先进行运算，然后再赋值
        if (operatorStr == "+=") {
            Object *p7ResObj = this->handelInfixExpression(p7NameObj, "+", p7ValueObj);
            p7env->EditVariable(name, p7ResObj);
        } else if (operatorStr == "-=") {
            Object *p7ResObj = this->handelInfixExpression(p7NameObj, "-", p7ValueObj);
            p7env->EditVariable(name, p7ResObj);
        } else if (operatorStr == "*=") {
            Object *p7ResObj = this->handelInfixExpression(p7NameObj, "*", p7ValueObj);
            p7env->EditVariable(name, p7ResObj);
        } else if (operatorStr == "/=") {
            Object *p7ResObj = this->handelInfixExpression(p7NameObj, "/", p7ValueObj);
            p7env->EditVariable(name, p7ResObj);
        } else if (operatorStr == "%=") {
            Object *p7ResObj = this->handelInfixExpression(p7NameObj, "%", p7ValueObj);
            p7env->EditVariable(name, p7ResObj);
        } else {
            //=，直接赋值
            p7env->EditVariable(name, p7ValueObj);
            return nullptr;
        }
    }
};

#endif //HKNI_INTERPRETER_H
