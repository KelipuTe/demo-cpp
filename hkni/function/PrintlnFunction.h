#ifndef HKNI_PRINTLN_FUNC_H
#define HKNI_PRINTLN_FUNC_H

#include <iostream>
#include <vector>
#include "../object/Object.h"
#include "../object/ErrorObject.h"
#include "../object/NullObject.h"

using namespace std;
using namespace objecthkni;

namespace functionhkni {
    //println
    Object *PrintlnFunc(std::vector<Object *> args) {
        if (args.empty()) {
            return new ErrorObject("println函数最少需要1个参数");
        }

        cout << args[0]->GetLiteral() << endl;

        return new NullObject();
    }
}
#endif
