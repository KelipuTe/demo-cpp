#ifndef HKNI_PARSER_H
#define HKNI_PARSER_H

#include <map>
#include <functional>

#include "Lexer.h"
#include "Precedence.h"

#include "ast/Program.h"

#include "expression/AssignExp.h"
#include "expression/BoolExp.h"
#include "expression/CallExp.h"
#include "expression/FloatExp.h"
#include "expression/ForExp.h"
#include "expression/FuncExp.h"
#include "expression/IdentifierExp.h"
#include "expression/IfExp.h"
#include "expression/InfixExp.h"
#include "expression/IntExp.h"
#include "expression/MapExp.h"
#include "expression/NullExp.h"
#include "expression/PrefixExp.h"
#include "expression/StringExp.h"

#include "statement/ExpStmt.h"
#include "statement/ReturnStmt.h"
#include "statement/VarStmt.h"

namespace hkni {
    //语法分析器，基于运算符优先级和递归的实现。
    class Parser {
    private:
        //类型别名
        using prefixParseF8 = std::function<ExpressionI9 *()>; //前缀标记的解析函数
        using infixParseF8 = std::function<ExpressionI9 *(ExpressionI9 *)>; //中缀标记的解析函数

        Lexer *lexerP7; //词法分析器

        Token nowToken; //当前token
        Token nextToken; //下一个token

        std::map<TokenType, PRECEDENCE> precedenceMap; //词法标记的优先级
        //如果在map里没找到，会返回类型的默认值0，0就是LOWEST_P，逻辑上不会出问题。

        std::map<TokenType, prefixParseF8> prefixParseF8Map; //前缀词法标记的解析函数
        std::map<TokenType, infixParseF8> infixParseF8Map; //中缀词法标记的解析函数
        //如果在map里没找到，会返回类型的默认值null。

        vector<string> errorList; //错误列表
    public:
        /**
         * @param lexerP7 需要传入一个词法分析器（词法分析完，才能进语法分析流程）
         */
        explicit Parser(Lexer *lexerP7) {
            this->lexerP7 = lexerP7;

            initPrecedence();
            initParsingFunction();

            //初始化nowToken和nextToken
            getNextToken();
            getNextToken();
        }

        //进行语法分析
        //返回的就是AST抽象语法树
        Program *DoParse() {
            auto *program = new Program();

            int i = 0;
            while (nowToken.TokenType != END) {
                if (nowToken.TokenType == ILLEGAL) {
                    reportError("非法的词法标记：" + nowToken.Literal + "。");
                    free(program);
                    return nullptr;
                }

                if (nowToken.TokenType == COMMENT) {
                    //遇到注释直接跳过
                    getNextToken();
                } else {
                    //解析语句，一个程序由多条语句组成
                    auto *stmtI9 = parseStmt();
                    if (stmtI9 != nullptr) {
                        program->StmtI9List.push_back(stmtI9);
                    }
                }

                i++;
                if (i > 10000) {
                    reportError("程序太长或者程序异常导致死循环。");
                    break;
                }
            }

            return program;
        }

        bool HasError() {
            return !errorList.empty();
        }

        void PrintError() {
            for (auto &err: errorList) {
                std::cout << err << std::endl;
            }
        }

    private:
        //初始化词法标记优先级。
        //写代码的顺序和TokenType那里的顺序一样，方便找。
        void initPrecedence() {
            precedenceMap[NOT] = NOT_SUB_DEC_INC_P; // !
            precedenceMap[NEQ] = NEQ_EQ_P; // !=
            precedenceMap[MOD] = MOD_MUL_DIV_P; // %
            precedenceMap[MOD_ASSIGN] = ASSIGN_P; // %=
            precedenceMap[BIT_AND] = BIT_AND_P; // &
            precedenceMap[AND] = AND_P; // &&
            precedenceMap[MUL] = MOD_MUL_DIV_P; // *
            precedenceMap[MUL_ASSIGN] = ASSIGN_P; // *=
            precedenceMap[LPAREN] = LPAREN_P; // (
            precedenceMap[SUB] = SUB_ADD_P; // -
            precedenceMap[SUB_ASSIGN] = ASSIGN_P; // -=
            precedenceMap[DEC] = NOT_SUB_DEC_INC_P; // --
            precedenceMap[ASSIGN] = ASSIGN_P; // =
            precedenceMap[EQ] = NEQ_EQ_P; // ==
            precedenceMap[ADD] = SUB_ADD_P; // +
            precedenceMap[ADD_ASSIGN] = ASSIGN_P; // +=
            precedenceMap[INC] = NOT_SUB_DEC_INC_P; // ++
            precedenceMap[BIT_OR] = BIT_OR_P; // |
            precedenceMap[OR] = OR_P; // ||
            precedenceMap[LT] = LT_LTE_GT_GTE_P; // <
            precedenceMap[LTE] = LT_LTE_GT_GTE_P; // <=
            precedenceMap[GT] = LT_LTE_GT_GTE_P; // >
            precedenceMap[GTE] = LT_LTE_GT_GTE_P; // >=
            precedenceMap[DIV] = MOD_MUL_DIV_P; // /
            precedenceMap[DIV_ASSIGN] = ASSIGN_P; // /=
        }

        //初始化词法标记的解析方法。
        //这里用 std::bind() 把每个 TokenType 对应的处理函数绑定给它
        //写代码的顺序和TokenType那里的顺序一样，方便找。
        void initParsingFunction() {
            //前缀

            //运算符
            prefixParseF8Map[STRING_VALUE] = std::bind(&Parser::parseStringExp, this); //字符串值
            prefixParseF8Map[NOT] = std::bind(&Parser::parsePrefixExp, this); //逻辑非
            prefixParseF8Map[SUB] = std::bind(&Parser::parsePrefixExp, this); //负号
            prefixParseF8Map[DEC] = std::bind(&Parser::parsePrefixExp, this); //自减
            prefixParseF8Map[INC] = std::bind(&Parser::parsePrefixExp, this); //自增
            prefixParseF8Map[LPAREN] = std::bind(&Parser::parseLParenExp, this); //左小括号（算术运算）

            //关键字
            prefixParseF8Map[BOOL_VALUE] = std::bind(&Parser::parseBoolExp, this); //布尔值
            prefixParseF8Map[INT_VALUE] = std::bind(&Parser::parseIntExp, this); //整数值
            prefixParseF8Map[FLOAT_VALUE] = std::bind(&Parser::parseFloatExp, this); //浮点数值
            prefixParseF8Map[NULL_HKNI] = std::bind(&Parser::parseNullExp, this); //null值
            prefixParseF8Map[IF_HKNI] = std::bind(&Parser::parseIfExp, this); //if语句
            prefixParseF8Map[FOR_HKNI] = std::bind(&Parser::parseForExp, this); //for循环语句
            prefixParseF8Map[FUNCTION_HKNI] = std::bind(&Parser::parseFuncExp, this); //函数定义
            prefixParseF8Map[MAP_HKNI] = std::bind(&Parser::parseMapExp, this); //map定义

            //其他
            prefixParseF8Map[IDENTIFIER] = std::bind(&Parser::parseIdentifierExp, this); //标识符

            //中缀只可能是运算符
            infixParseF8Map[NEQ] = std::bind(&Parser::parseInfixExp, this, std::placeholders::_1); //不等于
            infixParseF8Map[MOD] = std::bind(&Parser::parseInfixExp, this, std::placeholders::_1); //取模
            infixParseF8Map[MOD_ASSIGN] = std::bind(&Parser::parseAssignExp, this, std::placeholders::_1); //取模赋值
            infixParseF8Map[BIT_AND] = std::bind(&Parser::parseInfixExp, this, std::placeholders::_1); //位与
            infixParseF8Map[AND] = std::bind(&Parser::parseInfixExp, this, std::placeholders::_1); //逻辑与
            infixParseF8Map[MUL] = std::bind(&Parser::parseInfixExp, this, std::placeholders::_1); //乘法
            infixParseF8Map[MUL_ASSIGN] = std::bind(&Parser::parseAssignExp, this, std::placeholders::_1); //乘赋值
            infixParseF8Map[SUB] = std::bind(&Parser::parseInfixExp, this, std::placeholders::_1); //减法
            infixParseF8Map[SUB_ASSIGN] = std::bind(&Parser::parseAssignExp, this, std::placeholders::_1); //减赋值
            infixParseF8Map[ASSIGN] = std::bind(&Parser::parseAssignExp, this, std::placeholders::_1); //赋值
            infixParseF8Map[EQ] = std::bind(&Parser::parseInfixExp, this, std::placeholders::_1); //等于
            infixParseF8Map[ADD] = std::bind(&Parser::parseInfixExp, this, std::placeholders::_1); //加法
            infixParseF8Map[ADD_ASSIGN] = std::bind(&Parser::parseAssignExp, this, std::placeholders::_1); //加赋值
            infixParseF8Map[LPAREN] = std::bind(&Parser::parseCallExp, this, std::placeholders::_1); //左小括号（函数调用）
            infixParseF8Map[BIT_OR] = std::bind(&Parser::parseInfixExp, this, std::placeholders::_1); //位或
            infixParseF8Map[OR] = std::bind(&Parser::parseInfixExp, this, std::placeholders::_1); //逻辑或
            infixParseF8Map[LT] = std::bind(&Parser::parseInfixExp, this, std::placeholders::_1); //小于
            infixParseF8Map[LTE] = std::bind(&Parser::parseInfixExp, this, std::placeholders::_1); //小于等于
            infixParseF8Map[GT] = std::bind(&Parser::parseInfixExp, this, std::placeholders::_1); //大于
            infixParseF8Map[GTE] = std::bind(&Parser::parseInfixExp, this, std::placeholders::_1); //大于等于
            infixParseF8Map[DIV] = std::bind(&Parser::parseInfixExp, this, std::placeholders::_1); //除法
            infixParseF8Map[DIV_ASSIGN] = std::bind(&Parser::parseAssignExp, this, std::placeholders::_1); //除赋值
        }

        void getNextToken() {
            nowToken = nextToken;
            nextToken = lexerP7->GetNextToken();
        }

        bool nowTokenIs(TokenType type) {
            return nowToken.TokenType == type;
        }

        bool nextTokenIs(TokenType type) {
            return nextToken.TokenType == type;
        }

        //期望当前token是指定type类型的。
        //如果是，就返回true；如果不是，就返回false并报错；
        bool expectNowTokenIs(TokenType type) {
            if (nowTokenIs(type)) {
                return true;
            }
            reportError("expectNowTokenIs() error,nowTokenIs=" + nextToken.Literal + "。");
            return false;
        }

        //期望下一个token是指定type类型的。
        //如果是，就返回true并跳过当前token；如果不是，就返回false并报错；
        bool expectNextTokenIs(TokenType type) {
            if (nextTokenIs(type)) {
                getNextToken(); //跳到下一个token
                return true;
            }
            reportError("expectNextTokenIs() error,nextTokenIs=" + nextToken.Literal + "。");
            return false;
        }

        void reportError(string msg) {
            string errStr = "【语法分析错误】" + msg + "\n";
            string modeStr = lexerP7->GetModeStr();
            string rowStr = std::to_string(lexerP7->GetNowRow());
            string columnStr = std::to_string(lexerP7->GetNowColumn());
            string extStr = modeStr + rowStr + "行；" + columnStr + "列。\n";
            errorList.push_back(errStr + extStr);
        }

        int getNowTokenPrecedence() {
            return precedenceMap[nowToken.TokenType];
        }

        int getNextTokenPrecedence() {
            return precedenceMap[nextToken.TokenType];
        }

        //##解析语句的代码

        //解析语句
        StatementI9 *parseStmt() {
            switch (nowToken.TokenType) {
                case VAR_HKNI:
                    return parseVarStmt();
                case RETURN_HKNI:
                    return parseReturnStmt();
                default:
                    return parseExpStmt();
            }
        }

        //var语句
        VarStmt *parseVarStmt() {
            //now=var，next=标识符
            auto *stmt = new VarStmt();
            if (!expectNextTokenIs(IDENTIFIER)) {
                free(stmt);
                return nullptr;
            } //now=标识符，next=数据类型
            stmt->IdentifierExp = new IdentifierExp(nowToken);
            if (!nextTokenIs(BOOL_TYPE) &&
                !nextTokenIs(INT_TYPE) &&
                !nextTokenIs(FLOAT_TYPE) &&
                !nextTokenIs(STRING_TYPE) &&
                !nextTokenIs(MAP_HKNI)) {
                reportError("不支持的数据类型，nextTokenIs=" + nextToken.Literal + "。");
                free(stmt);
                return nullptr;
            }
            getNextToken(); //now=数据类型，next=;或=

            //这里需要额外处理数组、map 这几种
            stmt->TypeToken = nowToken;
            stmt->TypeTokenType = nowToken.TokenType;
            if (nowTokenIs(MAP_HKNI)) {
                stmt->TypeExp = parseMapExp();
            } else {
                this->getNextToken(); //now=;，next=其它代码，或，now='='，next=值的表达式
            }

            if (nowTokenIs(SEMICOLON)) {
                //now=;，next=其它代码。只声明没赋值
                this->getNextToken(); //跳过';'
                return stmt;
            }
            //now='='，next=值的表达式。声明并赋值。
            if (!expectNowTokenIs(ASSIGN)) {
                free(stmt);
                return nullptr;
            }
            this->getNextToken(); //跳过'='

            //now=值的表达式，next=;
            if (nowTokenIs(MAP_HKNI)) {
                stmt->TypeExp = parseMapExp();
            } else {
                stmt->ValExp = parseExp(LOWEST_P);
            }

            if (!expectNowTokenIs(SEMICOLON)) {
                free(stmt);
                return nullptr;
            }
            this->getNextToken(); //跳过';'
            return stmt;
        }

        //return语句
        ReturnStmt *parseReturnStmt() {
            //now=return,next=';'或者表达式
            auto *p7stmt = new ReturnStmt(nowToken);
            this->getNextToken(); //跳过return
            if (nowTokenIs(SEMICOLON)) {
                getNextToken(); //跳过';'
                return p7stmt;
            }
            p7stmt->I9Exp = parseExp(LOWEST_P);
            //now=';',next=
            if (!expectNowTokenIs(SEMICOLON)) {
                free(p7stmt);
                return nullptr;
            }
            getNextToken(); //跳过';'
            return p7stmt;
        }

        //表达式语句
        ExpStmt *parseExpStmt() {
            auto *p7stmt = new ExpStmt(nowToken);
            p7stmt->ExpI9 = parseExp(LOWEST_P);
            if (nowTokenIs(SEMICOLON)) {
                getNextToken(); //跳过';'
            }
            return p7stmt;
        }

        //块语句，会处理首尾的'{'和'}'
        BlockStmt *parseBlockStmt() {
            //now='{',next=
            auto *p7stmt = new BlockStmt();
            getNextToken(); //跳过'{'
            while (!nowTokenIs(RBRACE)) {
                //跳过注释
                if (nowTokenIs(COMMENT)) {
                    getNextToken();
                    continue;
                }
                //解析语句
                auto *i9stmt = parseStmt();
                if (i9stmt != nullptr) {
                    p7stmt->BodyStmtList.push_back(i9stmt);
                }
            }
            //now='}',next=
            getNextToken(); //跳过'}'
            return p7stmt;
        }

        //##解析语句的代码##

        //##解析表达式的代码

        //解析表达式
        //运算符的解析方法可能会递归调用该函数。
        ExpressionI9 *parseExp(int precedence) {
            //前缀解析方法
            auto f8Prefix = prefixParseF8Map[nowToken.TokenType];
            if (f8Prefix == nullptr) {
                reportError("未找到 " + nowToken.Literal + " 的前缀解析方法。");
                return nullptr;
            }
            auto leftExp = f8Prefix(); //调用前缀解析方法

            //如果当前token不是分号，而且当前的符号优先级比之前的符号优先级高，则优先按中缀解析
            while (!nowTokenIs(SEMICOLON) && precedence < getNowTokenPrecedence()) {
                //中缀解析方法
                auto f8Infix = infixParseF8Map[nowToken.TokenType];
                if (f8Infix == nullptr) {
                    reportError("未找到 " + nowToken.Literal + " 的中缀解析方法。");
                    return nullptr;
                }
                leftExp = f8Infix(leftExp); //调用中缀解析方法
            }

            return leftExp;
        }

        //##处理前缀结构的代码

        //前缀表达式，通用方法
        PrefixExp *parsePrefixExp() {
            auto *p7exp = new PrefixExp(nowToken);
            getNextToken(); //跳过前缀符号
            //这几个前缀符号的优先级基本是括号下面最高的优先级了
            p7exp->I9RightExp = parseExp(NOT_SUB_DEC_INC_P);
            return p7exp;
        }

        //字符串值
        StringExp *parseStringExp() {
            auto *exp = new StringExp(nowToken);
            getNextToken();
            return exp;
        }

        //左括号（算术运算）
        ExpressionI9 *parseLParenExp() {
            //now='(',next=表达式
            getNextToken(); //跳过'('
            auto *exp = parseExp(LOWEST_P);
            //now=')',next=后面的表达式
            if (!expectNowTokenIs(RPAREN)) {
                return nullptr;
            }
            getNextToken(); //跳过')'
            return exp;
        }

        //null值
        NullExp *parseNullExp() {
            auto *exp = new NullExp(nowToken);
            getNextToken();
            return exp;
        };

        //布尔值
        BoolExp *parseBoolExp() {
            auto *exp = new BoolExp(nowToken);
            getNextToken();
            return exp;
        };

        //整数值
        IntExp *parseIntExp() {
            auto *exp = new IntExp(nowToken);
            getNextToken();
            return exp;
        };

        //浮点数值
        FloatExp *parseFloatExp() {
            auto *exp = new FloatExp(nowToken);
            getNextToken();
            return exp;
        };

        //if语句
        IfExp *parseIfExp() {
            //now=if,next='('
            auto *p7exp = new IfExp();
            if (!expectNextTokenIs(LPAREN)) {
                return nullptr;
            }
            getNextToken(); //跳过'('
            //now='(',next=条件表达式
            p7exp->I9ConditionExp = parseExp(LOWEST_P);
            //now=')',next='{'
            if (!expectNowTokenIs(RPAREN)) {
                return nullptr;
            }
            if (!expectNextTokenIs(LBRACE)) {
                return nullptr;
            }
            //now='{',next=块语句
            p7exp->P7ConsequenceStmt = parseBlockStmt();
            //now=else,next='{'或者没有
            if (nowTokenIs(ELSE_HKNI)) {
                getNextToken(); //跳过else
                //now='{',next=块语句
                if (!expectNowTokenIs(LBRACE)) {
                    return nullptr;
                }
                p7exp->P7AlternativeStmt = parseBlockStmt();
            } else {
                p7exp->P7AlternativeStmt = nullptr;
            }
            return p7exp;
        }

        //for循环语句
        ForExp *parseForExp() {
            //now=for,next='('
            auto *p7exp = new ForExp();
            if (!expectNextTokenIs(LPAREN)) {
                return nullptr;
            }
            //now='(',next=初始化语句
            getNextToken();
            //now=初始化语句,next
            if (nowTokenIs(SEMICOLON)) {
                //初始化语句为空
            } else {
                p7exp->I9InitExp = parseExp(LOWEST_P);
            }
            getNextToken(); //跳过分号
            //now=条件表达式,next=
            if (nowTokenIs(SEMICOLON)) {
                //条件表达式为空
            } else {
                p7exp->I9ConditionExp = parseExp(LOWEST_P);
            }
            getNextToken(); //跳过分号
            //now=递增递减语句,next=
            if (nowTokenIs(RPAREN)) {
                //递增递减语句为空，什么都不做
            } else {
                p7exp->I9IncrementExp = parseExp(LOWEST_P);
            }
            //now=')',next='{'
            if (!expectNowTokenIs(RPAREN)) {
                return nullptr;
            }
            if (!expectNextTokenIs(LBRACE)) {
                return nullptr;
            }
            //now='{',next=块语句
            p7exp->P7BodyStmt = parseBlockStmt();

            return p7exp;
        }

        //函数定义
        FuncExp *parseFuncExp() {
            //now=关键字,next=函数名
            auto *p7exp = new FuncExp();
            if (nextTokenIs(IDENTIFIER)) {
                getNextToken();
                p7exp->P7NameExp = new IdentifierExp(nowToken); //处理函数名
            }
            //now=函数名,next='('
            if (!expectNextTokenIs(LPAREN)) {
                return nullptr;
            }
            this->getNextToken(); //跳过'('
            parseFuncArgsList(p7exp); //处理参数列表
            //now=')',next=返回值类型或者'{'
            if (!expectNowTokenIs(RPAREN)) {
                return nullptr;
            }
            if (nextTokenIs(BOOL_TYPE) || nextTokenIs(INT_TYPE) ||
                nextTokenIs(FLOAT_TYPE) || nextTokenIs(STRING_TYPE)) {
                getNextToken();
                p7exp->ReturnType = nowToken.TokenType; //处理返回值类型
            }
            //now=返回值类型,next='{'
            if (!expectNextTokenIs(LBRACE)) {
                return nullptr;
            }
            p7exp->P7BodyBlockStmt = parseBlockStmt(); //处理函数体
            return p7exp;
        }

        //函数定义的参数列表
        void parseFuncArgsList(FuncExp *p7exp) {
            //无参数
            if (nowTokenIs(RPAREN)) {
                return;
            }
            //有参数
            std::vector<IdentifierExp *> argList;
            std::vector<TokenType> argTypeList;

            //第一个参数
            argList.push_back(new IdentifierExp(nowToken)); //标识符
            this->getNextToken();
            argTypeList.push_back(nowToken.TokenType); //参数类型
            this->getNextToken();

            while (nowTokenIs(COMMA)) {
                this->getNextToken(); //跳过','

                //下一个参数
                argList.push_back(new IdentifierExp(nowToken));
                this->getNextToken();
                argTypeList.push_back(nowToken.TokenType);
                this->getNextToken();
            }

            p7exp->P7ArgTypeList = argTypeList;
            p7exp->P7ArgList = argList;
        }

        // TypeExp *parseTypeExpression() {
        //     //now="type"，next=类型标识符
        //
        //     auto *typeP7 = new TypeExp();
        //
        //     if (!expectNextTokenIs(IDENTIFIER)) {
        //         return nullptr;
        //     } //now=类型标识符，next=类型关键字
        //
        //     auto *nameP7 = new IdentifierExp(nowToken); //处理标识符
        //
        //     this->getNextToken(); //now=类型关键字，next='{'
        //
        //     if (!nowTokenIs(STRUCT_HKNI)) {
        //         reportError("类型定义语句解析错误，nowTokenIs=" + nowToken.Literal + "。");
        //         return nullptr;
        //     }
        //
        //     if (nowTokenIs(STRUCT_HKNI)) {
        //         //struct
        //         auto structP7 = new StructExp();
        //         structP7->NameP7 = nameP7;
        //
        //         if (!expectNextTokenIs(LBRACE)) {
        //             return nullptr;
        //         } //now='{'，next=字段标识符或者'}'
        //
        //         this->getNextToken(); //跳过'{'，now=字段标识符或者'}'，next=
        //
        //         parseStructField(structP7); //解析完，now='}'，next=
        //
        //         if (!expectNowTokenIs(RPAREN)) {
        //             return nullptr;
        //         }
        //
        //         typeP7->RealType = nowToken.TokenType;
        //         typeP7->RealExpP7 = structP7;
        //     }
        //
        //     return typeP7;
        // }

        MapExp *parseMapExp() {
            //now="map"，next=[
            auto *exp = new MapExp();
            if (!expectNextTokenIs(LBRACKET)) {
                reportError("【语法分析错误】map定义错误，nowTokenIs=" + nowToken.Literal + "。");
                return nullptr;
            } //now=[，next=类型关键字
            getNextToken(); //now=类型关键字，next=]
            exp->KeyExpI9 = nowToken.TokenType;
            if (!expectNextTokenIs(RBRACKET)) {
                reportError("【语法分析错误】map定义错误，nowTokenIs=" + nowToken.Literal + "。");
                return nullptr;
            } //now=]，next=类型关键字
            getNextToken(); //now=类型关键字，next=;或者{
            exp->ValExpI9 = nowToken.TokenType;

            getNextToken(); //now=;，next=其他代码，或者，now={，next=键数据
            if (nowTokenIs(LBRACE)) {
                //now={，next=键数据
                parseMapInitValList(exp);
                //now=}，next=;
                if (!nowTokenIs(RBRACE)) {
                    reportError("map语法错误，nowTokenIs=" + nowToken.Literal + "。");
                    return nullptr;
                }
                getNextToken(); //now=;，next=其他代码
            }

            if (!nowTokenIs(SEMICOLON)) {
                reportError("map语法错误，nowTokenIs=" + nowToken.Literal + "。");
                return nullptr;
            }

            return exp;
        }

        //解析map的初始值列表
        void parseMapInitValList(MapExp *exp) {
            //now={，next=键数据或者}
            getNextToken(); //now=}，next=;，或者，now=键数据，next=:
            if (nowTokenIs(RBRACE)) {
                //now=}，next=;
                return;
            }
            //now=键数据，next=:
            std::map<ExpressionI9 *, ExpressionI9 *> initValList;

            auto key = parseExp(LOWEST_P); //now=:，next=值数据
            if (!nowTokenIs(COLON)) {
                reportError("map语法错误，nowTokenIs=" + nowToken.Literal + "。");
                return;
            }
            getNextToken(); //now=值数据，next=,或者}
            auto val = parseExp(LOWEST_P); //now=}，next=;，或者，now=,，next=键数
            initValList[key] = val;

            while (nowTokenIs(COMMA)) {
                //now=,，next=键数据
                getNextToken(); //now=键数据，next=:
                auto key2 = parseExp(LOWEST_P); //now=:，next=值数据
                if (!nowTokenIs(COLON)) {
                    reportError("map语法错误，nowTokenIs=" + nowToken.Literal + "。");
                    return;
                }
                getNextToken(); //now=值数据，next=,或者}
                auto val2 = parseExp(LOWEST_P); //now=}，next=;，或者，now=,，next=键数
                initValList[key2] = val2;
            }

            exp->InitVal = initValList;
            //now=}，next=;
        }

        //解析结构体字段
        // void parseStructField(StructExp *structP7) {
        //     //无字段，now='}'，next=
        //     if (nowTokenIs(RPAREN)) {
        //         return;
        //     }
        //
        //     //有字段，now=字段标识符，next=字段类型
        //     std::map<string, StructField *> fieldP7Map;
        //
        //     StructField *field;
        //     while (true) {
        //         field = new StructField();
        //         field->FieldP7 = new IdentifierExp(nowToken); //字段标识符
        //
        //         this->getNextToken(); //now=字段类型，next=','
        //
        //         field->FieldType = nowToken.TokenType; //字段类型
        //         fieldP7Map[field->FieldP7->GetTokenLiteral()] = field;
        //
        //         this->getNextToken(); //now=','，next=字段标识符或者'}'
        //
        //         if (nowTokenIs(COMMA)) {
        //             continue;
        //         }
        //
        //         break;
        //     }
        //
        //     structP7->FieldP7Map = fieldP7Map;
        // }

        //解析标识符
        IdentifierExp *parseIdentifierExp() {
            auto *e = new IdentifierExp(nowToken);
            getNextToken();
            return e;
        }

        //##处理前缀结构的代码##

        //##处理中缀结构的代码

        //中缀表达式，通用方法
        InfixExp *parseInfixExp(ExpressionI9 *i9exp) {
            auto *p7exp = new InfixExp(nowToken);
            p7exp->I9LeftExp = i9exp;
            int p = this->getNowTokenPrecedence(); //获取中缀标记的优先级
            getNextToken(); //跳过中缀符号
            p7exp->I9RightExp = parseExp(p); //这里传的是中缀符号的优先级
            return p7exp;
        }

        //赋值
        AssignExp *parseAssignExp(ExpressionI9 *i9exp) {
            auto *p7exp = new AssignExp(nowToken);
            p7exp->I9NameExp = dynamic_cast<IdentifierExp *>(i9exp);
            getNextToken();
            p7exp->I9ValueExp = parseExp(LOWEST_P);
            return p7exp;
        }

        //左括号（函数调用）
        CallExp *parseCallExp(ExpressionI9 *funcEXP) {
            //now='(',next=参数列表或者')'
            auto *p7exp = new CallExp();
            p7exp->I9Exp = funcEXP; //处理函数名
            this->getNextToken(); //跳过'('
            parseCallArgsList(p7exp); //处理参数列表
            //now=')',next=
            if (!expectNowTokenIs(RPAREN)) {
                return nullptr;
            }
            this->getNextToken(); //跳过')'
            return p7exp;
        }

        //函数调用的参数列表
        void parseCallArgsList(CallExp *p7exp) {
            //无参数
            if (nowTokenIs(RPAREN)) {
                return;
            }
            //有参数
            std::vector<ExpressionI9 *> argList;

            argList.push_back(parseExp(LOWEST_P));
            while (nowTokenIs(COMMA)) {
                this->getNextToken(); //跳到','
                argList.push_back(parseExp(LOWEST_P));
            }
            p7exp->ArgExpList = argList;
        }

        //##处理中缀结构的代码##

        //##解析表达式的代码##
    };
}

#endif
