#ifndef HKNI_STATEMENT_H
#define HKNI_STATEMENT_H

#include "Node.h"

namespace ast {
    //语句抽象
    class I9Statement : public I9Node {
    public:
        virtual void StatementNode() = 0;
    };
}

#endif //HKNI_STATEMENT_H
