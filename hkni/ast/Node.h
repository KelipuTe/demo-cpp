#ifndef DEMO_CPP_NODE_H
#define DEMO_CPP_NODE_H

#include "../Token.h"

namespace ast {
    // ast语法树结点
    class C5Node {
    public:
        virtual string F8TokenLiteral() = 0;

        virtual string F8ToString() = 0;
    };
}


#endif //DEMO_CPP_NODE_H
