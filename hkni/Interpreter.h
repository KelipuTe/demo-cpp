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

#include "function/BuiltinFunction.h"

using namespace asthkni;
using namespace objecthkni;
using namespace functionhkni;

namespace hkni {
    class Interpreter {
    public:
        BuiltinFunc *p7BuiltinFunc; //内置函数

    public:
        //初始化内置函数
        void InitBuiltinFunc(BuiltinFunc *p7bf) {
            p7BuiltinFunc = p7bf;
        }

        //解释器入口，递归入口。
        //解释的过程可能会递归调用该函数。
        //顺序和语法分析那里一样。
        Object *DoInterpret(NodeI9 *i9node, Environment *p7env) {
            if (typeid(*i9node) == typeid(Program)) {
                auto *p7program = dynamic_cast<Program *>(i9node); //程序
                return interpretProgram(p7program, p7env);
            } else if (typeid(*i9node) == typeid(ExpressionStatement)) {
                auto *p7stmt = dynamic_cast<ExpressionStatement *>(i9node); //表达式语句
                return DoInterpret(p7stmt->I9Exp, p7env);
            } else if (typeid(*i9node) == typeid(ReturnStatement)) {
                auto *p7stmt = dynamic_cast<ReturnStatement *>(i9node); //return语句
                return interpretReturnStatement(p7stmt, p7env);
            } else if (typeid(*i9node) == typeid(BlockStatement)) {
                auto *p7stmt = dynamic_cast<BlockStatement *>(i9node); //块语句
                return interpretBlockStatement(p7stmt, p7env);
            } else if (typeid(*i9node) == typeid(VarStatement)) {
                auto *p7stmt = dynamic_cast<VarStatement *>(i9node); //var语句
                return interpretVarStatement(p7stmt, p7env);
            } else if (typeid(*i9node) == typeid(AssignExpression)) {
                auto *p7exp = dynamic_cast<AssignExpression *>(i9node); //赋值表达式
                return interpretAssignExpression(p7exp, p7env);
            } else if (typeid(*i9node) == typeid(BoolExpression)) {
                auto *p7exp = dynamic_cast<BoolExpression *>(i9node); //布尔表达式
                return new BoolObject(p7exp->GetTokenLiteral() == "true");
            } else if (typeid(*i9node) == typeid(CallExpression)) {
                auto *p7exp = dynamic_cast<CallExpression *>(i9node); //函数调用表达式
                return interpretCallExpression(p7exp, p7env);
            } else if (typeid(*i9node) == typeid(FloatExpression)) {
                auto *p7exp = dynamic_cast<FloatExpression *>(i9node); //浮点数表达式
                return new FloatObject(p7exp->GetValue());
            } else if (typeid(*i9node) == typeid(ForExpression)) {
                auto *p7exp = dynamic_cast<ForExpression *>(i9node); //for表达式
                return interpretForExpression(p7exp, p7env);
            } else if (typeid(*i9node) == typeid(FuncExpression)) {
                auto *p7exp = dynamic_cast<FuncExpression *>(i9node); //函数定义表达式
                return interpretFuncExpression(p7exp, p7env);
            } else if (typeid(*i9node) == typeid(IdentifierExpression)) {
                auto *p7exp = dynamic_cast<IdentifierExpression *>(i9node); //标识符表达式
                return interpretIdentifierExpression(p7exp, p7env);
            } else if (typeid(*i9node) == typeid(IfExpression)) {
                auto *p7exp = dynamic_cast<IfExpression *>(i9node); //if表达式
                return interpretIfExpression(p7exp, p7env);
            } else if (typeid(*i9node) == typeid(InfixExpression)) {
                auto *p7exp = dynamic_cast<InfixExpression *>(i9node); //中缀表达式
                return interpretInfixExpression(p7exp, p7env);
            } else if (typeid(*i9node) == typeid(IntExpression)) {
                auto *p7exp = dynamic_cast<IntExpression *>(i9node); //整数表达式
                return new IntObject(p7exp->GetValue());
            } else if (typeid(*i9node) == typeid(PrefixExpression)) {
                auto *p7exp = dynamic_cast<PrefixExpression *>(i9node); //前缀表达式
                return interpretPrefixExpression(p7exp, p7env);
            } else if (typeid(*i9node) == typeid(StringExpression)) {
                auto *p7exp = dynamic_cast<StringExpression *>(i9node); //字符串表达式
                return new StringObject(p7exp->GetTokenLiteral());
            }

            return nullptr;
        }

    private:
        //程序
        Object *interpretProgram(Program *p7program, Environment *p7env) {
            Object *p7obj = nullptr;
            for (auto i9stmt: p7program->StmtI9List) {
                p7obj = this->DoInterpret(i9stmt, p7env); //解释每一条语句
            }
            return p7obj;
        }

        //var语句
        Object *interpretVarStatement(VarStatement *p7stmt, Environment *p7env) {
            string name = p7stmt->P7IdentifierExp->GetTokenLiteral(); //变量名直接可以拿到
            Object *p7obj = nullptr;
            if (p7stmt->I9ValueExp != nullptr) {
                //如果有值表达式，就解释值表达式
                p7obj = this->DoInterpret(p7stmt->I9ValueExp, p7env);
            } else {
                //如果没有值表达式，就初始化0值
                if (p7stmt->ValueTokenType == BOOL_TYPE) {
                    p7obj = new BoolObject(false);
                } else if (p7stmt->ValueTokenType == INT_TYPE) {
                    p7obj = new IntObject(0);
                } else if (p7stmt->ValueTokenType == FLOAT_TYPE) {
                    p7obj = new FloatObject(0.0);
                } else if (p7stmt->ValueTokenType == STRING_TYPE) {
                    p7obj = new StringObject("");
                }
            }
            p7env->AddVariable(name, p7obj); //在环境中添加变量
            return nullptr;
        }

        //return语句
        Object *interpretReturnStatement(ReturnStatement *p7stmt, Environment *p7env) {
            Object *p7obj = this->DoInterpret(p7stmt->I9Exp, p7env);
            return new ReturnObject(p7obj);
        }

        //块语句
        Object *interpretBlockStatement(BlockStatement *p7stmt, Environment *p7env) {
            Object *p7obj = nullptr;
            for (auto i9stmt: p7stmt->BodyStmtList) {
                p7obj = this->DoInterpret(i9stmt, p7env); //解释每一条语句
                if (ObjectIs(p7obj, objecthkni::RETURN_OBJ) || ObjectIs(p7obj, objecthkni::ERROR_OBJ)) {
                    //如果解释语句的结果是return或者error，直接返回
                    return p7obj;
                }
            }
            return p7obj;
        }

        //赋值表达式
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
            }

            return nullptr;
        }

        //布尔中缀表达式
        Object *interpretBoolInfixExpression(Object *p7LeftObj, string operatorStr, Object *p7RightObj) {
            auto *p7LeftObjReal = dynamic_cast<BoolObject *>(p7LeftObj);
            auto *p7RightObjReal = dynamic_cast<BoolObject *>(p7RightObj);

            if (operatorStr == "&&") {
                return new BoolObject(p7LeftObjReal->Value && p7RightObjReal->Value);
            } else if (operatorStr == "||") {
                return new BoolObject(p7LeftObjReal->Value || p7RightObjReal->Value);
            } else {
                return new ErrorObject("错误。整数布尔不支持中缀操作符：" + operatorStr);
            }
        }

        //布尔缀表达式
        Object *interpretBoolPrefixExpression(string operatorStr, Object *p7obj) {
            auto *p7ObjReal = dynamic_cast<BoolObject *>(p7obj);
            if (operatorStr == "!") {
                return new BoolObject(!p7ObjReal->Value);
            } else {
                return new ErrorObject("错误。布尔类型不支持前缀操作符：" + operatorStr);
            }
        }

        //函数调用表达式
        Object *interpretCallExpression(CallExpression *p7exp, Environment *p7env) {
            //通过函数名找函数对象
            auto p7FuncObj = this->DoInterpret(p7exp->I9Exp, p7env);
            if (!ObjectIs(p7FuncObj, objecthkni::FUNC_OBJ)
                && !ObjectIs(p7FuncObj, objecthkni::BUILTIN_FUNC_OBJ)) {
                return new ErrorObject("错误。不是函数对象。");
            }

            //解析函数调用的参数列表
            auto argObjList = interpretCallExpressionArgExpList(p7exp->ArgExpList, p7env);

            //解析出来的参数个数和实际参数个数不一样，说明解析出错了
            if (argObjList.size() != p7exp->ArgExpList.size()) {
                return argObjList[p7exp->ArgExpList.size() - 1]; //错误对象默认放在最后
            }

            return this->callFunc(p7FuncObj, argObjList); //执行函数
        }

        //解析函数调用的参数列表
        std::vector<Object *>
        interpretCallExpressionArgExpList(std::vector<ExpressionI9 *> argExpList, Environment *p7env) {
            std::vector<Object *> argObjList;

            for (auto argExp: argExpList) {
                auto argObj = this->DoInterpret(argExp, p7env);
                if (ObjectIs(argObj, objecthkni::ERROR_OBJ)) {
                    //如果解释参数的时候出错了，直接返回错误对象
                    argObjList.push_back(argObj);
                    return argObjList;
                }
                argObjList.push_back(argObj);
            }

            return argObjList;
        }

        //调用函数
        Object *callFunc(Object *p7FuncObj, std::vector<Object *> argObjList) {
            if (typeid(*p7FuncObj) == typeid(FuncObject)) {
                auto p7FuncObjReal = dynamic_cast<FuncObject *>(p7FuncObj);
                auto p7FuncEnv = this->createFuncEnv(p7FuncObjReal, argObjList);
                return this->DoInterpret(p7FuncObjReal->P7BodyBlockStmt, p7FuncEnv);
            } else if (typeid(*p7FuncObj) == typeid(BuiltinFuncObject)) {
                auto builtin = dynamic_cast<BuiltinFuncObject *>(p7FuncObj);
                return builtin->Value(argObjList);
            }
            return new ErrorObject("错误。不是函数对象。");
        }

        //构造函数的环境
        Environment *createFuncEnv(FuncObject *p7FuncObj, std::vector<Object *> argObjList) {
            auto p7env = new Environment();
            p7env->P7OuterEnv = p7FuncObj->p7env; //函数的外层环境
            for (int i = 0; i < p7FuncObj->P7ArgList.size(); i++) {
                p7env->AddVariable(p7FuncObj->P7ArgList[i]->GetTokenLiteral(), argObjList[i]);
            }
            return p7env;
        }

        //浮点数中缀表达式
        Object *interpretFloatInfixExpression(Object *p7LeftObj, string operatorStr, Object *p7RightObj) {
            auto *p7LeftObjReal = dynamic_cast<FloatObject *>(p7LeftObj);
            auto *p7RightObjReal = dynamic_cast<FloatObject *>(p7RightObj);

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

        //浮点数前缀表达式
        Object *interpretFloatPrefixExpression(string operatorStr, Object *p7obj) {
            auto *p7ObjReal = dynamic_cast<FloatObject *>(p7obj);

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

        //for语句
        Object *interpretForExpression(ForExpression *p7exp, Environment *p7env) {
            Object *p7obj = nullptr;

            //for有自己的作用域
            auto *p7ForEnv = new Environment();
            p7ForEnv->P7OuterEnv = p7env;

            //初始化
            auto initObj = this->DoInterpret(p7exp->I9InitExp, p7ForEnv);
            if (ObjectIs(initObj, objecthkni::ERROR_OBJ)) {
                return initObj;
            }

            //执行循环
            Object *p7ConditionObj = nullptr;
            Object *incrementObj = nullptr;
            while (true) {
                //解释条件
                p7ConditionObj = this->DoInterpret(p7exp->I9ConditionExp, p7env);
                if (!ObjectIs(p7ConditionObj, objecthkni::BOOL_OBJ)) {
                    return new ErrorObject("错误。for语句的条件表达式结果不是布尔类型。");
                }
                auto *p7ConditionObjReal = dynamic_cast<BoolObject *>(p7ConditionObj);

                if (p7ConditionObjReal->Value) {
                    p7obj = this->DoInterpret(p7exp->P7BodyStmt, p7ForEnv);

                    if (ObjectIs(p7obj, objecthkni::ERROR_OBJ)) {
                        return p7obj;
                    }
                    //循环快里面遇到return，直接返回
                    if (ObjectIs(p7obj, objecthkni::RETURN_OBJ)) {
                        auto p7ReturnObj = dynamic_cast<ReturnObject *>(p7obj);
                        return p7ReturnObj->P7Value;
                    }

                    //解释递增递减
                    incrementObj = this->DoInterpret(p7exp->I9IncrementExp, p7ForEnv);
                    if (ObjectIs(incrementObj, objecthkni::ERROR_OBJ)) {
                        return incrementObj;
                    }
                } else {
                    break;
                }
            }

            p7ForEnv->VariableMap.clear(); //清空新的环境中的变量

            return p7obj;
        }

        //函数定义
        Object *interpretFuncExpression(FuncExpression *p7exp, Environment *p7env) {
            auto *p7obj = new FuncObject();

            p7obj->P7NameExp = p7exp->P7NameExp;
            p7obj->P7ArgList = p7exp->P7ArgList;
            p7obj->P7BodyBlockStmt = p7exp->P7BodyBlockStmt;
            p7obj->p7env = p7env;

            //函数名不为空，就把函数对象放到环境中
            if (p7exp->P7NameExp != nullptr) {
                string funcName = p7exp->P7NameExp->GetTokenLiteral();
                if (p7env->IsExistVariable(funcName)) {
                    return new ErrorObject("错误。函数【" + funcName + "】已经存在;");
                }
                p7env->AddVariable(funcName, p7obj);
            }

            return p7obj;
        }

        //标识符表达式
        Object *interpretIdentifierExpression(IdentifierExpression *p7exp, Environment *p7env) {
            string name = p7exp->GetTokenLiteral(); //拿到变量名
            auto p7obj = p7env->GetVariable(name); //去环境中获取变量
            if (p7obj != nullptr) {
                return p7obj;
            }
            //从内置函数模块找
            auto func = p7BuiltinFunc->GetBuiltinFunc(name);
            if (func != nullptr) {
                return func;
            }
            return new ErrorObject("错误。找不到变量：" + name);
        }


        //if语句
        Object *interpretIfExpression(IfExpression *p7exp, Environment *p7env) {
            Object *p7obj = nullptr;

            //if块和else块有自己的作用域
            auto *p7IfEnv = new Environment();
            p7IfEnv->P7OuterEnv = p7env;

            //解释条件表达式
            Object *p7ConditionObj = this->DoInterpret(p7exp->I9ConditionExp, p7env);
            if (!ObjectIs(p7ConditionObj, objecthkni::BOOL_OBJ)) {
                return new ErrorObject("错误。if语句的条件表达式结果不是布尔类型。");
            }
            auto *p7ConditionObjReal = dynamic_cast<BoolObject *>(p7ConditionObj);

            //根据条件表达式的结果，执行if块或者else块
            if (p7ConditionObjReal->Value) {
                if (p7exp->P7ConsequenceStmt != nullptr) {
                    p7obj = this->DoInterpret(p7exp->P7ConsequenceStmt, p7IfEnv);
                }
            } else {
                if (p7exp->P7AlternativeStmt != nullptr) {
                    p7obj = this->DoInterpret(p7exp->P7AlternativeStmt, p7IfEnv);
                }
            }

            p7IfEnv->VariableMap.clear(); //清空新的环境中的变量

            return p7obj;
        }


        //中缀表达式
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

            if (p7LeftObj->GetObjectType() != p7RightObj->GetObjectType()) {
                return new ErrorObject("错误。中缀表达式两边的对象类型不一样。");
            }

            if (ObjectIs(p7LeftObj, objecthkni::BOOL_OBJ)) {
                p7obj = this->interpretBoolInfixExpression(p7LeftObj, operatorStr, p7RightObj);
            } else if (ObjectIs(p7LeftObj, objecthkni::INT_OBJ)) {
                p7obj = this->interpretIntInfixExpression(p7LeftObj, operatorStr, p7RightObj);
            } else if (ObjectIs(p7LeftObj, objecthkni::FLOAT_OBJ)) {
                p7obj = this->interpretFloatInfixExpression(p7LeftObj, operatorStr, p7RightObj);
            }

            return p7obj;
        }


        //整数中缀表达式
        Object *interpretIntInfixExpression(Object *p7LeftObj, string operatorStr, Object *p7RightObj) {
            auto *p7LeftObjReal = dynamic_cast<IntObject *>(p7LeftObj);
            auto *p7RightObjReal = dynamic_cast<IntObject *>(p7RightObj);

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


        //整数前缀表达式
        Object *interpretIntPrefixExpression(string operatorStr, Object *p7obj) {
            auto *p7IntObj = dynamic_cast<IntObject *>(p7obj);

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


        //前缀表达式
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

            if (ObjectIs(p7RightObj, objecthkni::BOOL_OBJ)) {
                p7obj = this->interpretBoolPrefixExpression(operatorStr, p7RightObj);
            } else if (ObjectIs(p7RightObj, objecthkni::INT_OBJ)) {
                p7obj = this->interpretIntPrefixExpression(operatorStr, p7RightObj);
            } else if (ObjectIs(p7RightObj, objecthkni::FLOAT_OBJ)) {
                p7obj = this->interpretFloatPrefixExpression(operatorStr, p7RightObj);
            }

            return p7obj;
        }
    };
}

#endif
