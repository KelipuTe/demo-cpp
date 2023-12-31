#ifndef HKNI_NODE_H
#define HKNI_NODE_H

namespace ast {
    //AST树结点抽象
    class I9Node {
    public:
        //获取词法标记原始值
        virtual string GetTokenLiteral() = 0;

        virtual string ToString() = 0;
    };
}


#endif //HKNI_NODE_H
