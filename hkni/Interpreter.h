#ifndef HKNI_INTERPRETER_H
#define HKNI_INTERPRETER_H

#include <typeinfo>

#include "ast/Program.h"

#include "Environment.h"

#include "object/IntObject.h"
#include "object/FloatObject.h"
#include "object/StringObject.h"
#include "object/ErrorObject.h"

using namespace ast;
using namespace env;
using namespace object;

class Interpreter {
public:
    //解释器入口（递归入口）
    Object *DoInterpret(I9Node *i9node, Environment *p7env) {
        if (typeid(*i9node) == typeid(Program)) {
            auto *p7program = dynamic_cast<Program *>(i9node);
            return interpretProgram(p7program, p7env);
        } else if (typeid(*i9node) == typeid(VarStatement)) {
            auto *p7stmt = dynamic_cast<VarStatement *>(i9node);
            return interpretVarStatement(p7stmt, p7env);
        } else if (typeid(*i9node) == typeid(ExpressionStatement)) {
            auto *p7stmt = dynamic_cast<ExpressionStatement *>(i9node);
            return DoInterpret(p7stmt->I9Exp, p7env);
        } else if (typeid(*i9node) == typeid(IntExpression)) {
            auto *p7exp = dynamic_cast<IntExpression *>(i9node);
            return new IntObject(p7exp->GetValue());
        } else if (typeid(*i9node) == typeid(FloatExpression)) {
            auto *p7exp = dynamic_cast<FloatExpression *>(i9node);
            return new FloatObject(p7exp->GetValue());
        } else if (typeid(*i9node) == typeid(StringExpression)) {
            auto *p7exp = dynamic_cast<StringExpression *>(i9node);
            return new StringObject(p7exp->GetTokenLiteral());
        }else if (typeid(*i9node) == typeid(AssignExpression)) {
            auto *p7exp = dynamic_cast<AssignExpression *>(i9node);
            return interpretAssignExpression(p7exp,p7env);
        }
        return nullptr;
    }

    Object *interpretProgram(Program *p7program, Environment *p7env) {
        Object *p7obj = nullptr;
        for (auto i9stmt: p7program->I9StatementList) {
            p7obj = this->DoInterpret(i9stmt, p7env);
        }
        return p7obj;
    }

    Object *interpretVarStatement(VarStatement *p7stmt, Environment *p7env) {
        string name = p7stmt->P7NameExp->GetTokenLiteral();
        auto p7ValueObj = this->DoInterpret(p7stmt->I9ValueExp, p7env);
        p7env->AddVariable(name, p7ValueObj);
        return nullptr;
    }

    Object *interpretAssignExpression(AssignExpression *p7exp, Environment *p7env) {
        string name = p7exp->I9NameExp->GetTokenLiteral();
        if(!p7env->ExistVariable(name)){
            return new ErrorObject("想要赋值，先要有变量。");
        }
        auto p7ValueObj = this->DoInterpret(p7exp->I9ValueExp, p7env);
        p7env->SetVariable(name, p7ValueObj);
        return nullptr;
    }
};

#endif //HKNI_INTERPRETER_H
