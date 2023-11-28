#ifndef HKNI_NODE_H
#define HKNI_NODE_H

namespace ast {
    //AST树结点
    class I9Node {
    public:
        virtual string GetTokenLiteral() = 0;

        virtual string ToString() = 0;
    };
}


#endif //HKNI_NODE_H
