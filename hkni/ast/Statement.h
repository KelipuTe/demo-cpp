#ifndef DEMO_CPP_STATEMENT_H
#define DEMO_CPP_STATEMENT_H

#include "Node.h"

namespace ast {
    //语句
    class I9Statement : public I9Node {
    public:
        virtual void StatementNode() = 0;
    };
}

#endif //DEMO_CPP_STATEMENT_H
