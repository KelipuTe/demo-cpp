#ifndef HKNI_NODE_H
#define HKNI_NODE_H

namespace asthkni {
    //AST抽象语法树结点抽象
    class I9Node {
    public:
        //获取词法标记原始值
        virtual string GetTokenLiteral() = 0;

        //将结点以程序代码的形式输出
        virtual string ToString() = 0;
    };
}

#endif
