#ifndef HKNI_INTERPRETER_H
#define HKNI_INTERPRETER_H

#include <typeinfo>

#include "ast/Program.h"

#include "Environment.h"

#include "object/BoolObject.h"
#include "object/ErrorObject.h"
#include "object/FloatObject.h"
#include "object/FuncObject.h"
#include "object/IntObject.h"
#include "object/ReturnObject.h"
#include "object/StringObject.h"

#include "func/BuiltinFunc.h"

using namespace ast;
using namespace env;
using namespace object;
using namespace func;

class Interpreter {
    //##属性
public:
    BuiltinFunc* _builtinFunctions;
    void registerBuiltinFunctions(BuiltinFunc* builtinFunctions) {
        _builtinFunctions = builtinFunctions;
    }
    //##方法
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
        } else if (typeid(*i9node) == typeid(BlockStatement)) {
            auto *p7stmt = dynamic_cast<BlockStatement *>(i9node); //块语句
            return interpretBlockStatement(p7stmt, p7env);
        } else if (typeid(*i9node) == typeid(PrefixExpression)) {
            auto *p7exp = dynamic_cast<PrefixExpression *>(i9node); //前缀表达式
            return interpretPrefixExpression(p7exp, p7env);
        } else if (typeid(*i9node) == typeid(InfixExpression)) {
            auto *p7exp = dynamic_cast<InfixExpression *>(i9node); //中缀表达式
            return interpretInfixExpression(p7exp, p7env);
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
        } else if (typeid(*i9node) == typeid(IdentifierExpression)) {
            auto *p7exp = dynamic_cast<IdentifierExpression *>(i9node); //标识符表达式
            return interpretIdentifierExpression(p7exp, p7env);
        } else if (typeid(*i9node) == typeid(AssignExpression)) {
            auto *p7exp = dynamic_cast<AssignExpression *>(i9node); //赋值表达式
            return interpretAssignExpression(p7exp, p7env);
        } else if (typeid(*i9node) == typeid(IfExpression)) {
            auto *p7exp = dynamic_cast<IfExpression *>(i9node); //if表达式
            return interpretIfExpression(p7exp, p7env);
        } else if (typeid(*i9node) == typeid(FuncExpression)) {
            auto *p7exp = dynamic_cast<FuncExpression *>(i9node); //函数声明表达式
            return interpretFuncExpression(p7exp, p7env);
        } else if (typeid(*i9node) == typeid(CallExpression)) {
            auto *p7exp = dynamic_cast<CallExpression *>(i9node); //函数调用表达式
            return interpretCallExpression(p7exp, p7env);
        }else if (typeid(*i9node) == typeid(ReturnStatement)) {
            auto *p7stmt = dynamic_cast<ReturnStatement *>(i9node); //返回语句
            return interpretReturnStatement(p7stmt,p7env);
        }
        return nullptr;
    }

private:
    //判断对象是不是某种类型
    bool objectIs(Object *p7obj, OBJECT_TYPE type) {
        if (p7obj == nullptr) {
            return false;
        }
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
            if (p7stmt->ValueType == BOOL_TYPE) {
                p7ValueObj = new BoolObject(false);
            } else if (p7stmt->ValueType == INT_TYPE) {
                p7ValueObj = new IntObject(0);
            } else if (p7stmt->ValueType == FLOAT_TYPE) {
                p7ValueObj = new FloatObject(0.0);
            } else if (p7stmt->ValueType == STRING_TYPE) {
                p7ValueObj = new StringObject("");
            }
        }
        p7env->AddVariable(name, p7ValueObj); //在环境中添加变量
        return nullptr;
    }

    Object *interpretBlockStatement(BlockStatement *p7stmt, Environment *p7env) {
        Object *p7obj = nullptr;
        for (auto i9stmt: p7stmt->BodyStmtList) {
            p7obj = this->DoInterpret(i9stmt, p7env); //解释每一条语句
            if (objectIs(p7obj, object::RETURN_OBJ) || objectIs(p7obj, object::ERROR_OBJ)) {
                //如果解释语句的结果是return或者error，直接返回
                return p7obj;
            }
        }
        return p7obj;
    }

    //解释前缀表达式
    Object *interpretPrefixExpression(PrefixExpression *p7exp, Environment *p7env) {
        auto p7RightObj = this->DoInterpret(p7exp->I9RightExp, p7env);

        return this->handelPrefixExpression(p7exp->OperatorStr, p7RightObj);
    }

    //处理前缀表达式
    Object *handelPrefixExpression(string operatorStr, Object *p7RightObj) {
        Object *p7obj = nullptr;

        if (p7RightObj == nullptr) {
            return new ErrorObject("错误。前缀表达式为空");
        }

        if (objectIs(p7RightObj, object::BOOL_OBJ)) {
            p7obj = this->interpretBoolPrefixExpression(operatorStr, p7RightObj);
        } else if (objectIs(p7RightObj, object::INT_OBJ)) {
            p7obj = this->interpretIntPrefixExpression(operatorStr, p7RightObj);
        } else if (objectIs(p7RightObj, object::FLOAT_OBJ)) {
            p7obj = this->interpretFloatPrefixExpression(operatorStr, p7RightObj);
        }

        return p7obj;
    }

    //解释布尔前缀表达式
    Object *interpretBoolPrefixExpression(string operatorStr, Object *p7obj) {
        IntObject *p7ObjReal = dynamic_cast<IntObject *>(p7obj);

        if (operatorStr == "!") {
            return new IntObject(-p7ObjReal->Value);
        } else {
            return new ErrorObject("错误。布尔类型不支持前缀操作符：" + operatorStr);
        }
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
            return new ErrorObject("错误。整数类型不支持前缀操作符：" + operatorStr);
        }
    }

    //解释浮点数前缀表达式
    Object *interpretFloatPrefixExpression(string operatorStr, Object *p7obj) {
        FloatObject *p7ObjReal = dynamic_cast<FloatObject *>(p7obj);

        if (operatorStr == "-") {
            return new FloatObject(-p7ObjReal->Value);
        } else if (operatorStr == "++") {
            return new FloatObject(++p7ObjReal->Value); //执行操作后，变量的值会变
        } else if (operatorStr == "--") {
            return new FloatObject(--p7ObjReal->Value); //执行操作后，变量的值会变
        } else {
            return new ErrorObject("错误。浮点数类型不支持前缀操作符：" + operatorStr);
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

        if (p7LeftObj == nullptr || p7RightObj == nullptr) {
            return new ErrorObject("错误。中缀表达式为空");
        }

        if (p7LeftObj->GetType() != p7RightObj->GetType()) {
            return new ErrorObject("错误。中缀表达式两边的对象类型不一样。");
        }

        if (objectIs(p7LeftObj, object::BOOL_OBJ)) {
            p7obj = this->interpretBoolInfixExpression(p7LeftObj, operatorStr, p7RightObj);
        } else if (objectIs(p7LeftObj, object::INT_OBJ)) {
            p7obj = this->interpretIntInfixExpression(p7LeftObj, operatorStr, p7RightObj);
        } else if (objectIs(p7LeftObj, object::FLOAT_OBJ)) {
            p7obj = this->interpretFloatInfixExpression(p7LeftObj, operatorStr, p7RightObj);
        }

        return p7obj;
    }

    //解释布尔中缀表达式
    Object *interpretBoolInfixExpression(Object *p7LeftObj, string operatorStr, Object *p7RightObj) {
        BoolObject *p7LeftObjReal = dynamic_cast<BoolObject *>(p7LeftObj);
        BoolObject *p7RightObjReal = dynamic_cast<BoolObject *>(p7RightObj);

        if (operatorStr == "&&") {
            return new BoolObject(p7LeftObjReal->Value && p7RightObjReal->Value);
        } else if (operatorStr == "||") {
            return new BoolObject(p7LeftObjReal->Value || p7RightObjReal->Value);
        } else {
            return new ErrorObject("错误。整数布尔不支持中缀操作符：" + operatorStr);
        }
    }

    //解释整数中缀表达式
    Object *interpretIntInfixExpression(Object *p7LeftObj, string operatorStr, Object *p7RightObj) {
        IntObject *p7LeftObjReal = dynamic_cast<IntObject *>(p7LeftObj);
        IntObject *p7RightObjReal = dynamic_cast<IntObject *>(p7RightObj);

        if (operatorStr == "+") {
            return new IntObject(p7LeftObjReal->Value + p7RightObjReal->Value);
        } else if (operatorStr == "-") {
            return new IntObject(p7LeftObjReal->Value - p7RightObjReal->Value);
        } else if (operatorStr == "*") {
            return new IntObject(p7LeftObjReal->Value * p7RightObjReal->Value);
        } else if (operatorStr == "/") {
            return new IntObject(p7LeftObjReal->Value / p7RightObjReal->Value);
        } else if (operatorStr == "%") {
            return new IntObject(p7LeftObjReal->Value % p7RightObjReal->Value);
        } else if (operatorStr == "==") {
            return new BoolObject(p7LeftObjReal->Value == p7RightObjReal->Value);
        } else if (operatorStr == "!=") {
            return new BoolObject(p7LeftObjReal->Value != p7RightObjReal->Value);
        } else if (operatorStr == ">") {
            return new BoolObject(p7LeftObjReal->Value > p7RightObjReal->Value);
        } else if (operatorStr == "<") {
            return new BoolObject(p7LeftObjReal->Value < p7RightObjReal->Value);
        } else if (operatorStr == ">=") {
            return new BoolObject(p7LeftObjReal->Value >= p7RightObjReal->Value);
        } else if (operatorStr == "<=") {
            return new BoolObject(p7LeftObjReal->Value <= p7RightObjReal->Value);
        } else if (operatorStr == "&") {
            return new IntObject(p7LeftObjReal->Value & p7RightObjReal->Value);
        } else if (operatorStr == "|") {
            return new IntObject(p7LeftObjReal->Value | p7RightObjReal->Value);
        } else {
            return new ErrorObject("错误。整数类型不支持中缀操作符：" + operatorStr);
        }
    }

    //解释浮点数中缀表达式
    Object *interpretFloatInfixExpression(Object *p7LeftObj, string operatorStr, Object *p7RightObj) {
        FloatObject *p7LeftObjReal = dynamic_cast<FloatObject *>(p7LeftObj);
        FloatObject *p7RightObjReal = dynamic_cast<FloatObject *>(p7RightObj);

        //c++里面可以用运算符比较浮点数，但是这里用常规的相减之后和0比的办法。
        if (operatorStr == "+") {
            return new FloatObject(p7LeftObjReal->Value + p7RightObjReal->Value);
        } else if (operatorStr == "-") {
            return new FloatObject(p7LeftObjReal->Value - p7RightObjReal->Value);
        } else if (operatorStr == "*") {
            return new FloatObject(p7LeftObjReal->Value * p7RightObjReal->Value);
        } else if (operatorStr == "/") {
            return new FloatObject(p7LeftObjReal->Value / p7RightObjReal->Value);
        } else if (operatorStr == "==") {
            return new BoolObject((p7LeftObjReal->Value - p7RightObjReal->Value) < 0.000001);
        } else if (operatorStr == "!=") {
            return new BoolObject((p7LeftObjReal->Value - p7RightObjReal->Value) > 0.000001);
        } else if (operatorStr == ">") {
            return new BoolObject((p7LeftObjReal->Value - p7RightObjReal->Value) > 0);
        } else if (operatorStr == "<") {
            return new BoolObject((p7LeftObjReal->Value - p7RightObjReal->Value) < 0);
        } else {
            return new ErrorObject("错误。浮点数类型不支持中缀操作符：" + operatorStr);
        }
    }

    //解释标识符表达式
    Object *interpretIdentifierExpression(IdentifierExpression *p7exp, Environment *p7env) {
        string name = p7exp->GetTokenLiteral(); //拿到变量名
        auto p7obj = p7env->GetVariable(name); //去环境中获取变量
        if (p7obj != nullptr) {
            return p7obj;
        }
        //从内置函数模块找
        auto func = _builtinFunctions->getBuiltinFunc(name);
        if (func != nullptr) {
            return func;
        }
        return new ErrorObject("错误。找不到变量：" + name);
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

    Object *interpretIfExpression(IfExpression *p7exp, Environment *p7env) {
        Object *p7obj = nullptr;

        //解释条件表达式
        Object *p7ConditionObj = this->DoInterpret(p7exp->I9ConditionExp, p7env);
        if (!objectIs(p7ConditionObj, object::BOOL_OBJ)) {
            return new ErrorObject("错误。if语句的条件表达式结果不是布尔类型。");
        }
        BoolObject *p7ConditionObjReal = dynamic_cast<BoolObject *>(p7ConditionObj);

        //根据条件表达式的结果，执行if块或者else块
        //这里需要一个新的环境，因为if块和else块有自己的作用域
        Environment *p7IfEnv = new Environment();
        p7IfEnv->p7outer = p7env;
        if (p7ConditionObjReal->Value) {
            if (p7exp->P7ConsequenceStmt != nullptr) {
                p7obj = this->DoInterpret(p7exp->P7ConsequenceStmt, p7IfEnv);
            }
        } else {
            if (p7exp->P7AlternativeStmt != nullptr) {
                p7obj = this->DoInterpret(p7exp->P7AlternativeStmt, p7IfEnv);
            }
        }
        p7IfEnv->variableMap.clear(); //清空新的环境中的变量

        return p7obj;
    }

    Object *interpretFuncExpression(FuncExpression *p7exp, Environment *p7env) {

        FuncObject *p7obj = new FuncObject();

        p7obj->P7NameExp = p7exp->P7NameExp;
        p7obj->P7ArgList = p7exp->P7ArgList;
        p7obj->P7BodyBlockStmt = p7exp->P7BodyBlockStmt;
        p7obj->p7env = p7env;

        //函数名不为空，就把函数对象放到环境中
        if (p7exp->P7NameExp != nullptr) {
            string funcName = p7exp->P7NameExp->GetTokenLiteral();
            if (p7env->IsExistVariable(funcName)) {
                return new ErrorObject("错误。函数名已经存在：" + funcName);
            }
            p7env->AddVariable(funcName, p7obj);
        }

        return p7obj;
    }

    Object *interpretCallExpression(CallExpression *p7exp, Environment *p7env) {

        auto p7FuncObj = this->DoInterpret(p7exp->I9Exp, p7env); //通过函数名得到函数对象

        if (!objectIs(p7FuncObj, object::FUNC_OBJ)
        && !objectIs(p7FuncObj, object::BUILTIN_FUNC_OBJ)) {
            return new ErrorObject("错误。不是函数对象。");
        }

        auto argObjList = interpretExpressionList(p7exp->ArgExpList, p7env); //解析函数参数列表

        if (argObjList.size() != p7exp->ArgExpList.size()) {
            //解析出来的参数个数和实际参数个数不一样，说明解析出错了，最后一个参数是错误对象
            return argObjList[p7exp->ArgExpList.size() - 1];
        }

        return this->callFunc(p7FuncObj, argObjList); //执行函数
    }

    std::vector<Object *> interpretExpressionList(std::vector<I9Expression *> argExpList, Environment *p7env) {
        std::vector<Object *> argObjList;

        for (auto argExp: argExpList) {
            auto argObj = this->DoInterpret(argExp, p7env);
            if (objectIs(argObj, object::ERROR_OBJ)) {
                //如果解释参数的时候出错了，直接返回错误对象
                argObjList.push_back(argObj);
                return argObjList;
            }
            argObjList.push_back(argObj);
        }

        return argObjList;
    }

    Object *callFunc(Object *p7FuncObj, std::vector<Object *> argObjList) {
        if (typeid(*p7FuncObj) == typeid(FuncObject)) {
            auto *p7FuncObjReal = dynamic_cast<FuncObject *>(p7FuncObj);
            auto p7FuncEnv = this->createFuncEnv(p7FuncObjReal, argObjList);
            return this->DoInterpret(p7FuncObjReal->P7BodyBlockStmt, p7FuncEnv);
        }else if(typeid(*p7FuncObj) == typeid(BuiltinFuncObj)){
            BuiltinFuncObj* builtin = dynamic_cast<BuiltinFuncObj*>(p7FuncObj);
            return builtin->_func(argObjList);
        }
        return new ErrorObject("错误。不是函数对象。");
    }

    Environment *createFuncEnv(FuncObject *p7FuncObj, std::vector<Object *> argObjList) {
        auto p7env = new Environment();
        p7env->p7outer = p7FuncObj->p7env; //函数的外层环境
        for (int i = 0; i < p7FuncObj->P7ArgList.size(); i++) {
            p7env->AddVariable(p7FuncObj->P7ArgList[i]->GetTokenLiteral(), argObjList[i]);
        }
        return p7env;
    }

    Object *interpretReturnStatement(ReturnStatement *p7stmt, Environment *p7env) {
        Object *p7obj = this->DoInterpret(p7stmt->I9Exp, p7env);
        return new ReturnObject(p7obj);
    }
};

#endif //HKNI_INTERPRETER_H
