#ifndef HKNI_PRINTLN_FUNC_H
#define HKNI_PRINTLN_FUNC_H

#include <iostream>
#include <vector>

#include "../object/Object.h"
#include "../object/ErrorObject.h"
#include "../object/NullObject.h"

namespace hkni {
    //println
    Object *PrintlnFunc(std::vector<Object *> args) {
        if (args.empty()) {
            return new ErrorObj("println函数最少需要1个参数");
        }

        for (int i = 0; i < args.size(); i++) {
            cout << args[i]->GetLiteral() << endl;
        }

        return new NullObject();
    }
}
#endif
