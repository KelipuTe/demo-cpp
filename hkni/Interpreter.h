#ifndef HKNI_INTERPRETER_H
#define HKNI_INTERPRETER_H

#include <typeinfo>

#include "ast/Program.h"

#include "object/Object.h"
#include "object/IntObject.h"

#include "Environment.h"

using namespace ast;
using namespace object;
using namespace env;

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
        }
    }

    Object *interpretProgram(Program *p7program, Environment *p7env) {
        Object *p7obj = nullptr;
        for (auto i9stmt: p7program->I9StatementList) {
            p7obj = this->DoInterpret(i9stmt, p7env);
        }
        return p7obj;
    }

    Object *interpretVarStatement(VarStatement *p7stmt, Environment *p7env) {
        string name = p7stmt->P7NameExp->Value;
        auto p7ValueObj = this->DoInterpret(p7stmt->I9ValueExp, p7env);
        p7env->SetVariable(name, p7ValueObj);
        return nullptr;
    }
};

#endif //HKNI_INTERPRETER_H
