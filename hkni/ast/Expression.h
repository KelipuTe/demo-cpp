#ifndef DEMO_CPP_EXPRESSION_H
#define DEMO_CPP_EXPRESSION_H

#include "Node.h"

namespace ast {
    //表达式
    class I9Expression : public I9Node {
    public:
        virtual void ExpressionNode() = 0;
    };
}

#endif //DEMO_CPP_EXPRESSION_H
