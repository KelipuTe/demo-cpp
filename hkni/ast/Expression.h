#ifndef HKNI_EXPRESSION_H
#define HKNI_EXPRESSION_H

#include "Node.h"

namespace ast {
    //表达式抽象
    class I9Expression : public I9Node {
    public:
        virtual void ExpressionNode() = 0;
    };
}

#endif //HKNI_EXPRESSION_H
