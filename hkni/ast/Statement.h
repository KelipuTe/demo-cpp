#ifndef HKNI_STATEMENT_H
#define HKNI_STATEMENT_H

#include "Node.h"

namespace hkni {
    //语句抽象
    class StatementI9 : public NodeI9 {
    public:
        virtual void StatementNode() = 0;
    };
}

#endif
