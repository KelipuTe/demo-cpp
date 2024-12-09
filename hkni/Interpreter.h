#ifndef HKNI_INTERPRETER_H
#define HKNI_INTERPRETER_H

#include <typeinfo>

#include "ast/Program.h"

#include "statement/ReturnStmt.h"
#include "statement/VarStmt.h"

#include "object/BoolObj.h"
#include "object/ErrorObj.h"
#include "object/FloatObj.h"
#include "object/FuncObj.h"
#include "object/IntObj.h"
#include "object/ReturnObj.h"
#include "object/StringObj.h"

#include "function/BuiltinFunc.h"

#include "Environment.h"

namespace hkni {
    class Interpreter {
        //方法声明

        //解释布尔中缀表达式
        Object *itptBoolInfixExp(Object *left, string op, Object *right);

        //解释布尔前缀表达式
        Object *itptBoolPrefixExp(string op, Object *obj);

        //解释浮点数中缀表达式
        Object *itptFloatInfixExp(Object *left, string op, Object *right);

        //解释浮点数前缀表达式
        Object *itptFloatPrefixExp(string op, Object *obj);

        //解释整数中缀表达式
        Object *itptIntInfixExp(Object *left, string op, Object *right);

        //解释整数前缀表达式
        Object *itptIntPrefixExp(string op, Object *obj);

        //解释块语句
        Object *itptBlockStmt(BlockStmt *stmt, Environment *env);

        //解释return语句
        Object *itptReturnStmt(ReturnStmt *stmt, Environment *env);

        //解释var语句
        Object *itptVarStmt(VarStmt *stmt, Environment *env);

    public:
        BuiltinFunc *builtinFuncP7; //内置函数

    public:
        //初始化内置函数
        void InitBuiltinFunc(BuiltinFunc *bf) {
            builtinFuncP7 = bf;
        }

        //解释执行程序（AST抽象语法树）
        //解释的过程可能会递归调用该函数
        //写代码的顺序和语法分析那里一样，方便找。
        Object *DoInterpret(NodeI9 *nodeI9, Environment *env) {
            //解释表达式
            if (typeid(*nodeI9) == typeid(AssignExp)) {
                auto *expP7 = dynamic_cast<AssignExp *>(nodeI9); //赋值表达式
                return interpretAssignExpression(expP7, env);
            }
            if (typeid(*nodeI9) == typeid(BoolExp)) {
                auto *expP7 = dynamic_cast<BoolExp *>(nodeI9); //布尔表达式
                return new BoolObj(expP7->GetTokenLiteral() == "true");
            }
            if (typeid(*nodeI9) == typeid(CallExp)) {
                auto *expP7 = dynamic_cast<CallExp *>(nodeI9); //函数调用表达式
                return interpretCallExpression(expP7, env);
            }
            if (typeid(*nodeI9) == typeid(FloatExp)) {
                auto *expP7 = dynamic_cast<FloatExp *>(nodeI9); //浮点数表达式
                return new FloatObj(expP7->GetValue());
            }
            if (typeid(*nodeI9) == typeid(ForExp)) {
                auto *expP7 = dynamic_cast<ForExp *>(nodeI9); //for表达式
                return interpretForExpression(expP7, env);
            }
            if (typeid(*nodeI9) == typeid(FuncExp)) {
                auto *expP7 = dynamic_cast<FuncExp *>(nodeI9); //函数定义表达式
                return interpretFuncExpression(expP7, env);
            }
            if (typeid(*nodeI9) == typeid(IdentifierExp)) {
                auto *expP7 = dynamic_cast<IdentifierExp *>(nodeI9); //标识符表达式
                return interpretIdentifierExpression(expP7, env);
            }
            if (typeid(*nodeI9) == typeid(IfExp)) {
                auto *expP7 = dynamic_cast<IfExp *>(nodeI9); //if表达式
                return interpretIfExpression(expP7, env);
            }
            if (typeid(*nodeI9) == typeid(InfixExp)) {
                auto *expP7 = dynamic_cast<InfixExp *>(nodeI9); //中缀表达式
                return interpretInfixExpression(expP7, env);
            }
            if (typeid(*nodeI9) == typeid(IntExp)) {
                auto *expP7 = dynamic_cast<IntExp *>(nodeI9); //整数表达式
                return new IntObj(expP7->GetValue());
            }
            if (typeid(*nodeI9) == typeid(PrefixExp)) {
                auto *expP7 = dynamic_cast<PrefixExp *>(nodeI9); //前缀表达式
                return interpretPrefixExpression(expP7, env);
            }
            if (typeid(*nodeI9) == typeid(StringExp)) {
                auto *expP7 = dynamic_cast<StringExp *>(nodeI9); //字符串表达式
                return new StringObj(expP7->GetTokenLiteral());
            }

            //解释语句
            if (typeid(*nodeI9) == typeid(BlockStmt)) {
                auto *stmtP7 = dynamic_cast<BlockStmt *>(nodeI9); //块语句
                return itptBlockStmt(stmtP7, env);
            }
            if (typeid(*nodeI9) == typeid(ExpStmt)) {
                auto *stmtP7 = dynamic_cast<ExpStmt *>(nodeI9); //表达式语句
                return DoInterpret(stmtP7->ExpI9, env); //这里是递归结构
            }
            if (typeid(*nodeI9) == typeid(ReturnStmt)) {
                auto *stmtP7 = dynamic_cast<ReturnStmt *>(nodeI9); //return语句
                return itptReturnStmt(stmtP7, env);
            }
            if (typeid(*nodeI9) == typeid(VarStmt)) {
                auto *stmtP7 = dynamic_cast<VarStmt *>(nodeI9); //var语句
                return itptVarStmt(stmtP7, env);
            }

            //解释程序
            if (typeid(*nodeI9) == typeid(Program)) {
                auto *programP7 = dynamic_cast<Program *>(nodeI9); //程序
                return itptProgram(programP7, env);
            }

            return nullptr;
        }

    private:
        //解释程序
        Object *itptProgram(Program *programP7, Environment *envP7) {
            Object *objP7 = nullptr;
            for (auto stmtI9: programP7->StmtI9List) {
                objP7 = this->DoInterpret(stmtI9, envP7); //解释每一条语句
            }
            return objP7;
        }

        //解释赋值表达式
        Object *interpretAssignExpression(AssignExp *expP7, Environment *p7env) {
            string name = expP7->I9NameExp->GetTokenLiteral(); //拿到变量名
            auto p7NameObj = this->DoInterpret(expP7->I9NameExp, p7env); //先从环境中获取变量
            string operatorStr = expP7->operationStr; //拿到操作符
            auto p7ValueObj = this->DoInterpret(expP7->I9ValueExp, p7env); //解释变量值

            //运算并赋值的这几个，都需要先进行运算，然后再赋值
            if (operatorStr == "+=") {
                Object *p7ResObj = this->handelInfixExpression(p7NameObj, "+", p7ValueObj);
                p7env->EditVar(name, p7ResObj);
            } else if (operatorStr == "-=") {
                Object *p7ResObj = this->handelInfixExpression(p7NameObj, "-", p7ValueObj);
                p7env->EditVar(name, p7ResObj);
            } else if (operatorStr == "*=") {
                Object *p7ResObj = this->handelInfixExpression(p7NameObj, "*", p7ValueObj);
                p7env->EditVar(name, p7ResObj);
            } else if (operatorStr == "/=") {
                Object *p7ResObj = this->handelInfixExpression(p7NameObj, "/", p7ValueObj);
                p7env->EditVar(name, p7ResObj);
            } else if (operatorStr == "%=") {
                Object *p7ResObj = this->handelInfixExpression(p7NameObj, "%", p7ValueObj);
                p7env->EditVar(name, p7ResObj);
            } else {
                //=，直接赋值
                p7env->EditVar(name, p7ValueObj);
            }

            return nullptr;
        }


        //解释函数调用表达式
        Object *interpretCallExpression(CallExp *expP7, Environment *p7env) {
            //通过函数名找函数对象
            auto p7FuncObj = this->DoInterpret(expP7->I9Exp, p7env);
            if (!ObjectIs(p7FuncObj, FUNC_OBJ)
                && !ObjectIs(p7FuncObj, BUILTIN_FUNC_OBJ)) {
                return new ErrorObj("错误。不是函数对象。");
            }

            //解析函数调用的参数列表
            auto argObjList = interpretCallExpressionArgExpList(expP7->ArgExpList, p7env);

            //解析出来的参数个数和实际参数个数不一样，说明解析出错了
            if (argObjList.size() != expP7->ArgExpList.size()) {
                return argObjList[expP7->ArgExpList.size() - 1]; //错误对象默认放在最后
            }

            return this->callFunc(p7FuncObj, argObjList); //执行函数
        }

        //解释函数调用表达式的参数列表
        std::vector<Object *>
        interpretCallExpressionArgExpList(std::vector<ExpressionI9 *> argExpList, Environment *p7env) {
            std::vector<Object *> argObjList;

            for (auto argExp: argExpList) {
                auto argObj = this->DoInterpret(argExp, p7env);
                if (ObjectIs(argObj, ERROR_OBJ)) {
                    //如果解释参数的时候出错了，直接返回错误对象
                    argObjList.push_back(argObj);
                    return argObjList;
                }
                argObjList.push_back(argObj);
            }

            return argObjList;
        }

        //构造函数的局部环境
        Environment *createFuncEnv(FuncObj *p7FuncObj, std::vector<Object *> argObjList) {
            auto p7env = new Environment();
            p7env->outerEnv = p7FuncObj->p7env; //函数的外层环境
            for (int i = 0; i < p7FuncObj->P7ArgList.size(); i++) {
                p7env->AddVar(p7FuncObj->P7ArgList[i]->GetTokenLiteral(), argObjList[i]);
            }
            return p7env;
        }

        //调用函数
        Object *callFunc(Object *p7FuncObj, std::vector<Object *> argObjList) {
            if (typeid(*p7FuncObj) == typeid(FuncObj)) {
                auto p7FuncObjReal = dynamic_cast<FuncObj *>(p7FuncObj);
                auto p7FuncEnv = this->createFuncEnv(p7FuncObjReal, argObjList);
                return this->DoInterpret(p7FuncObjReal->P7BodyBlockStmt, p7FuncEnv);
            }
            if (typeid(*p7FuncObj) == typeid(BuiltinFuncObj)) {
                auto builtin = dynamic_cast<BuiltinFuncObj *>(p7FuncObj);
                return builtin->Value(argObjList);
            }
            return new ErrorObj("错误。不是函数对象。");
        }

        //解释for语句表达式
        Object *interpretForExpression(ForExp *expP7, Environment *p7env) {
            Object *p7obj = nullptr;

            //for有自己的作用域
            auto *p7ForEnv = new Environment();
            p7ForEnv->outerEnv = p7env;

            //初始化
            auto initObj = this->DoInterpret(expP7->I9InitExp, p7ForEnv);
            if (ObjectIs(initObj, ERROR_OBJ)) {
                return initObj;
            }

            //执行循环
            Object *p7ConditionObj = nullptr;
            Object *incrementObj = nullptr;
            while (true) {
                //解释条件
                p7ConditionObj = this->DoInterpret(expP7->I9ConditionExp, p7env);
                if (!ObjectIs(p7ConditionObj, BOOL_OBJ)) {
                    return new ErrorObj("错误。for语句的条件表达式结果不是布尔类型。");
                }
                auto *p7ConditionObjReal = dynamic_cast<BoolObj *>(p7ConditionObj);

                if (p7ConditionObjReal->Value) {
                    p7obj = this->DoInterpret(expP7->P7BodyStmt, p7ForEnv);

                    if (ObjectIs(p7obj, ERROR_OBJ)) {
                        return p7obj;
                    }
                    //循环快里面遇到return，直接返回
                    if (ObjectIs(p7obj, RETURN_OBJ)) {
                        auto p7ReturnObj = dynamic_cast<ReturnObj *>(p7obj);
                        return p7ReturnObj->P7Value;
                    }

                    //解释递增递减
                    incrementObj = this->DoInterpret(expP7->I9IncrementExp, p7ForEnv);
                    if (ObjectIs(incrementObj, ERROR_OBJ)) {
                        return incrementObj;
                    }
                } else {
                    break;
                }
            }

            p7ForEnv->VarMap.clear(); //清空新的环境中的变量

            return p7obj;
        }

        //解释函数定义表达式
        Object *interpretFuncExpression(FuncExp *expP7, Environment *p7env) {
            auto *p7obj = new FuncObj();

            p7obj->P7NameExp = expP7->P7NameExp;
            p7obj->P7ArgList = expP7->P7ArgList;
            p7obj->P7BodyBlockStmt = expP7->P7BodyBlockStmt;
            p7obj->p7env = p7env;

            //函数名不为空，就把函数对象放到环境中
            if (expP7->P7NameExp != nullptr) {
                string funcName = expP7->P7NameExp->GetTokenLiteral();
                if (p7env->IsVarExist(funcName)) {
                    return new ErrorObj("错误。函数【" + funcName + "】已经存在;");
                }
                p7env->AddVar(funcName, p7obj);
            }

            return p7obj;
        }

        //解释标识符表达式
        Object *interpretIdentifierExpression(IdentifierExp *expP7, Environment *p7env) {
            string name = expP7->GetTokenLiteral(); //拿到变量名
            auto p7obj = p7env->GetVar(name); //去环境中获取变量
            if (p7obj != nullptr) {
                return p7obj;
            }
            //从内置函数模块找
            auto func = builtinFuncP7->GetBuiltinFunc(name);
            if (func != nullptr) {
                return func;
            }
            return new ErrorObj("错误。找不到变量：" + name);
        }


        //解释if语句
        Object *interpretIfExpression(IfExp *expP7, Environment *p7env) {
            Object *p7obj = nullptr;

            //if块和else块有自己的作用域
            auto *p7IfEnv = new Environment();
            p7IfEnv->outerEnv = p7env;

            //解释条件表达式
            Object *p7ConditionObj = this->DoInterpret(expP7->I9ConditionExp, p7env);
            if (!ObjectIs(p7ConditionObj, BOOL_OBJ)) {
                return new ErrorObj("错误。if语句的条件表达式结果不是布尔类型。");
            }
            auto *p7ConditionObjReal = dynamic_cast<BoolObj *>(p7ConditionObj);

            //根据条件表达式的结果，执行if块或者else块
            if (p7ConditionObjReal->Value) {
                if (expP7->P7ConsequenceStmt != nullptr) {
                    p7obj = this->DoInterpret(expP7->P7ConsequenceStmt, p7IfEnv);
                }
            } else {
                if (expP7->P7AlternativeStmt != nullptr) {
                    p7obj = this->DoInterpret(expP7->P7AlternativeStmt, p7IfEnv);
                }
            }

            p7IfEnv->VarMap.clear(); //清空新的环境中的变量

            return p7obj;
        }


        //解释中缀表达式
        Object *interpretInfixExpression(InfixExp *expP7, Environment *p7env) {
            auto p7LeftObj = this->DoInterpret(expP7->I9LeftExp, p7env);
            auto p7RightObj = this->DoInterpret(expP7->I9RightExp, p7env);

            return this->handelInfixExpression(p7LeftObj, expP7->OperatorStr, p7RightObj);
        }

        //处理中缀表达式
        Object *handelInfixExpression(Object *p7LeftObj, string operatorStr, Object *p7RightObj) {
            Object *p7obj = nullptr;

            if (p7LeftObj == nullptr || p7RightObj == nullptr) {
                return new ErrorObj("错误。中缀表达式为空");
            }

            if (p7LeftObj->GetObjectType() != p7RightObj->GetObjectType()) {
                return new ErrorObj("错误。中缀表达式两边的对象类型不一样。");
            }

            if (ObjectIs(p7LeftObj, BOOL_OBJ)) {
                p7obj = itptBoolInfixExp(p7LeftObj, operatorStr, p7RightObj);
            } else if (ObjectIs(p7LeftObj, INT_OBJ)) {
                p7obj = itptIntInfixExp(p7LeftObj, operatorStr, p7RightObj);
            } else if (ObjectIs(p7LeftObj, FLOAT_OBJ)) {
                p7obj = itptFloatInfixExp(p7LeftObj, operatorStr, p7RightObj);
            }

            return p7obj;
        }

        //解释前缀表达式
        Object *interpretPrefixExpression(PrefixExp *expP7, Environment *p7env) {
            auto p7RightObj = this->DoInterpret(expP7->I9RightExp, p7env);

            return this->handelPrefixExpression(expP7->OperatorStr, p7RightObj);
        }

        //处理前缀表达式
        Object *handelPrefixExpression(string operatorStr, Object *p7RightObj) {
            Object *p7obj = nullptr;
            if (p7RightObj == nullptr) {
                return new ErrorObj("错误。前缀表达式为空");
            }

            if (ObjectIs(p7RightObj, BOOL_OBJ)) {
                p7obj = this->itptBoolPrefixExp(operatorStr, p7RightObj);
            } else if (ObjectIs(p7RightObj, INT_OBJ)) {
                p7obj = this->itptIntPrefixExp(operatorStr, p7RightObj);
            } else if (ObjectIs(p7RightObj, FLOAT_OBJ)) {
                p7obj = this->itptFloatPrefixExp(operatorStr, p7RightObj);
            }

            return p7obj;
        }
    };
}

#endif
