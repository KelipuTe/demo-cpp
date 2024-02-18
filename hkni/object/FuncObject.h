#ifndef HKNI_FUNC_OBJECT_H
#define HKNI_FUNC_OBJECT_H

#include "Object.h"

#include "../exp/IdentifierExpression.h"
#include "../stmt/BlockStatement.h"

#include "../Environment.h"

using namespace ast;
using namespace env;

namespace object {
    class FuncObject : public Object {
        //####属性
    public:
        OBJECT_TYPE Type;
        IdentifierExpression *P7NameExp; //函数名
        std::vector<IdentifierExpression*> P7ArgList; //参数列表
        BlockStatement* P7BodyBlockStmt; //函数体
        Environment *p7env; //函数作用域
        //####方法
    public:
        FuncObject() {
            this->Type = FUNC_OBJ;
        }

        OBJECT_TYPE GetType() override {
            return this->Type;
        }

        string GetLiteral() override {
        }

        string ToString() override {
            string t4str;
            return t4str;
        }
    };
}

#endif //HKNI_FUNC_OBJECT_H
