#ifndef DEMO_CPP_NODE_H
#define DEMO_CPP_NODE_H

#include "../Token.h"

namespace ast {
    //结点
    class I9Node {
    public:
        virtual string GetTokenLiteral() = 0;

        virtual string ToString() = 0;
    };
}


#endif //DEMO_CPP_NODE_H
