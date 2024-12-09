//这个文件放解释语句（stmt）的代码

#include "../Interpreter.h"

namespace hkni {
    Object *Interpreter::itptBlockStmt(BlockStmt *stmt, Environment *env) {
        Object *p7obj = nullptr;
        for (auto i9stmt: stmt->BodyStmtList) {
            p7obj = this->DoInterpret(i9stmt, env); //解释每一条语句
            if (ObjectIs(p7obj, RETURN_OBJ) || ObjectIs(p7obj, ERROR_OBJ)) {
                //如果解释语句的结果是return或者error，直接返回
                return p7obj;
            }
        }
        return p7obj;
    }

    Object *Interpreter::itptReturnStmt(ReturnStmt *stmt, Environment *env) {
        Object *p7obj = this->DoInterpret(stmt->I9Exp, env);
        return new ReturnObj(p7obj);
    }

    Object *Interpreter::itptVarStmt(VarStmt *stmt, Environment *env) {
        string name = stmt->IdentifierExp->GetTokenLiteral(); //变量名直接可以拿到
        Object *objP7 = nullptr;
        if (stmt->ValExp != nullptr) {
            //如果有值表达式，就解释值表达式
            objP7 = this->DoInterpret(stmt->ValExp, env);
        } else {
            //如果没有值表达式，就初始化0值
            if (stmt->TypeTokenType == BOOL_TYPE) {
                objP7 = new BoolObj(false);
            } else if (stmt->TypeTokenType == INT_TYPE) {
                objP7 = new IntObj(0);
            } else if (stmt->TypeTokenType == FLOAT_TYPE) {
                objP7 = new FloatObj(0.0);
            } else if (stmt->TypeTokenType == STRING_TYPE) {
                objP7 = new StringObj("");
            }
        }
        env->AddVar(name, objP7); //在环境中添加变量
        return nullptr;
    }
}
