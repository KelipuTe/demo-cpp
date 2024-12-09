#ifndef HKNI_MAP_EXP_H
#define HKNI_MAP_EXP_H

#include <string>
#include <map>

#include "../Token.h"
#include "../ast/Expression.h"

namespace hkni {
    //map达式
    //只声明没初始值：map[键数据类型]值数据类型
    //声明并赋初始值：map[键数据类型]值数据类型{键数据:值数据,键数据:值数据,...}
    class MapExp : public ExpressionI9 {
    public:
        TokenType KeyExpI9; //键数据类型
        TokenType ValExpI9; //值数据类型
        std::map<ExpressionI9 *, ExpressionI9 *> InitVal; //初始值
    public:
        MapExp() {
        }

        string GetTokenLiteral() override {
            return "map";
        }

        string ToString() override {
            string str;

            str.append("map[");
            str.append(TokenTypeToString(KeyExpI9));
            str.append("]");
            str.append(TokenTypeToString(ValExpI9));
            if (!InitVal.empty()) {
                str.append("{\n");
                for (auto item: InitVal) {
                    str.append(item.first->ToString());
                    str.append(":");
                    str.append(item.second->ToString());
                    str.append(",\n");
                }
                str.append("}");
            }

            return str;
        }

        void ExpressionNode() override {
        }
    };
}

#endif
