#ifndef HKNI_PARSER_H
#define HKNI_PARSER_H

#include <map>
#include <functional>

#include "Lexer.h"
#include "Precedence.h"

#include "expression/AssignExpression.h"
#include "expression/BoolExpression.h"
#include "expression/CallExpression.h"
#include "expression/FloatExpression.h"
#include "expression/ForExpression.h"
#include "expression/FuncExpression.h"
#include "expression/IdentifierExpression.h"
#include "expression/IfExpression.h"
#include "expression/InfixExpression.h"
#include "expression/IntExpression.h"
#include "expression/NullExpression.h"
#include "expression/PrefixExpression.h"
#include "expression/StringExpression.h"
#include "expression/SuffixExpression.h"

#include "statement/ExpressionStatement.h"
#include "statement/ReturnStatement.h"
#include "statement/VarStatement.h"

#include "ast/Program.h"

using namespace ast;

namespace hkni {
    //语法分析器，基于运算符优先级和递归的实现。
    class Parser {
        //##属性
    private:
        //类型别名
        using f8PrefixParsing = std::function<I9Expression * ()>; //前缀标记的解析函数
        using f8InfixParsing = std::function<I9Expression * (I9Expression * )>; //中缀标记的解析函数

        Lexer *p7lexer; //词法分析器

        Token nowToken; //当前token
        Token nextToken; //下一个token

        std::map<TokenType, PRECEDENCE> precedenceMap; //词法标记的优先级
        //这里不需要用map的find函数判断key是否存在。如果在map里没找到，会返回默认值0，0就是LOWEST_P，逻辑上不会出问题。

        std::map<TokenType, f8PrefixParsing> f8PrefixParsingMap; //前缀词法标记的解析函数
        std::map<TokenType, f8InfixParsing> f8InfixParsingMap; //中缀词法标记的解析函数

        vector<string> errorList; //错误列表
        //##方法
    public:
        Parser(Lexer *p7lexer) {
            this->p7lexer = p7lexer;

            initPrecedence();
            initParsingFunction();

            //初始化nowToken和nextToken
            getNextToken();
            getNextToken();
        };

        Program *DoParse() {
            auto *p7program = new Program();

            int i = 0;
            while (nowToken.TokenType != END) {
                if (nowToken.TokenType == ILLEGAL) {
                    reportError("语法错误：" + nowToken.Literal + "。");
                    return nullptr;
                } else if (nowToken.TokenType == COMMENT) {
                    getNextToken();
                } else {
                    //解析语句，一个程序由多条语句组成
                    I9Statement *i9stmt = this->parseStatement();
                    if (i9stmt != nullptr) {
                        p7program->I9StatementList.push_back(i9stmt);
                    }
                }
                ++i;
                if (i > 10000) {
                    reportError("程序太长或者程序异常导致死循环。");
                    break;
                }
            }

            return p7program;
        };

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

        //初始化词法标记的解析方法，前缀和中缀两个map。如果在map里没找到，会返回默认值null。
        void initParsingFunction() {
            //前缀
            //运算符
            f8PrefixParsingMap[STRING_VALUE] = std::bind(&Parser::parseStringExpression, this); //字符串值
            f8PrefixParsingMap[NOT] = std::bind(&Parser::parsePrefixExpression, this); //逻辑非
            f8PrefixParsingMap[SUB] = std::bind(&Parser::parsePrefixExpression, this); //负号
            f8PrefixParsingMap[DEC] = std::bind(&Parser::parsePrefixExpression, this); //自减
            f8PrefixParsingMap[INC] = std::bind(&Parser::parsePrefixExpression, this); //自增
            f8PrefixParsingMap[LPAREN] = std::bind(&Parser::parseLParenExpression, this); //左括号（算术运算）
            //关键字
            f8PrefixParsingMap[NULL_HKNI] = std::bind(&Parser::parseNullExpression, this); //null值
            f8PrefixParsingMap[BOOL_VALUE] = std::bind(&Parser::parseBoolExpression, this); //布尔值
            f8PrefixParsingMap[INT_VALUE] = std::bind(&Parser::parseIntExpression, this); //整数值
            f8PrefixParsingMap[FLOAT_VALUE] = std::bind(&Parser::parseFloatExpression, this); //浮点数值
            f8PrefixParsingMap[IF_HKNI] = std::bind(&Parser::parseIfExpression, this); //if语句
            f8PrefixParsingMap[FOR_HKNI] = std::bind(&Parser::parseForExpression, this); //for循环语句
            f8PrefixParsingMap[FUNCTION_HKNI] = std::bind(&Parser::parseFuncExpression, this); //函数定义
            //其他
            f8PrefixParsingMap[IDENTIFIER] = std::bind(&Parser::parseIdentifierExpression, this); //标识符

            //中缀
            //运算符
            f8InfixParsingMap[NEQ] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //不等于
            f8InfixParsingMap[MOD] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //取模
            f8InfixParsingMap[MOD_ASSIGN] =
                    std::bind(&Parser::parseAssignExpression, this, std::placeholders::_1); //取模赋值
            f8InfixParsingMap[BIT_AND] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //位与
            f8InfixParsingMap[AND] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //逻辑与
            f8InfixParsingMap[MUL] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //乘法
            f8InfixParsingMap[MUL_ASSIGN] =
                    std::bind(&Parser::parseAssignExpression, this, std::placeholders::_1); //乘赋值
            f8InfixParsingMap[SUB] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //减法
            f8InfixParsingMap[SUB_ASSIGN] =
                    std::bind(&Parser::parseAssignExpression, this, std::placeholders::_1); //减赋值
            f8InfixParsingMap[ASSIGN] = std::bind(&Parser::parseAssignExpression, this, std::placeholders::_1); //赋值
            f8InfixParsingMap[EQ] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //等于
            f8InfixParsingMap[ADD] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //加法
            f8InfixParsingMap[ADD_ASSIGN] =
                    std::bind(&Parser::parseAssignExpression, this, std::placeholders::_1); //加赋值
            f8InfixParsingMap[LPAREN] =
                    std::bind(&Parser::parseCallExpression, this, std::placeholders::_1); //左括号（函数调用）
            f8InfixParsingMap[BIT_OR] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //位或
            f8InfixParsingMap[OR] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //逻辑或
            f8InfixParsingMap[LT] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //小于
            f8InfixParsingMap[LTE] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //小于等于
            f8InfixParsingMap[GT] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //大于
            f8InfixParsingMap[GTE] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //大于等于
            f8InfixParsingMap[DIV] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //除法
            f8InfixParsingMap[DIV_ASSIGN] =
                    std::bind(&Parser::parseAssignExpression, this, std::placeholders::_1); //除赋值
        }

        void getNextToken() {
            nowToken = nextToken;
            nextToken = p7lexer->GetNextToken();
        };

        bool nowTokenIs(TokenType type) {
            return nowToken.TokenType == type;
        };

        bool nextTokenIs(TokenType type) {
            return nextToken.TokenType == type;
        };

        //期望当前token是指定type类型的。
        //如果是就返回true；如果不是就返回false并报错；
        bool expectNowTokenIs(TokenType type) {
            if (nowTokenIs(type)) {
                return true;
            } else {
                reportError("expectNowTokenIs error,TokenHKNI=" + nextToken.Literal + "。");
                return false;
            }
        }

        //期望下一个token是指定type类型的。
        //如果是就返回true并跳过当前token；如果不是就返回false并报错；
        bool expectNextTokenIs(TokenType type) {
            if (nextTokenIs(type)) {
                getNextToken();
                return true;
            } else {
                reportError("expectNextTokenIs error,TokenHKNI=" + nextToken.Literal + "。");
                return false;
            }
        }

        void reportError(string msg) {
            string modeStr = this->p7lexer->GetModeStr();
            string rowStr = std::to_string(this->p7lexer->GetNowRow());
            string columnStr = std::to_string(this->p7lexer->GetNowColumn());
            string str = modeStr + rowStr + "行；" + columnStr + "列。" + msg;
            errorList.push_back(str);
        }

        int getNowTokenPrecedence() {
            return precedenceMap[nowToken.TokenType];
        }

        int getNextTokenPrecedence() {
            return precedenceMap[nextToken.TokenType];
        }

        //##解析表达式

        //运算符的解析方法可能会递归调用parseExpression
        I9Expression *parseExpression(int precedence) {
            //前缀解析方法
            auto f8Prefix = f8PrefixParsingMap[nowToken.TokenType];
            if (f8Prefix == nullptr) {
                reportError("未找到 " + nowToken.Literal + " 的前缀解析方法。");
                return nullptr;
            }
            auto leftExp = f8Prefix(); //调用前缀解析方法

            //如果当前token不是分号，而且当前的符号优先级比之前的符号优先级高，则优先按中缀解析
            while (!nowTokenIs(SEMICOLON) && precedence < getNowTokenPrecedence()) {
                //中缀解析方法
                auto f8Infix = f8InfixParsingMap[nowToken.TokenType];
                if (f8Infix == nullptr) {
                    reportError("未找到 " + nowToken.Literal + " 的中缀解析方法。");
                    return nullptr;
                }
                leftExp = f8Infix(leftExp);//调用中缀解析方法
            }

            return leftExp;
        }

        //##前缀
        //前缀表达式，通用方法
        PrefixExpression *parsePrefixExpression() {
            auto *p7exp = new PrefixExpression(nowToken);
            getNextToken(); //跳过前缀符号
            //这几个前缀符号的优先级基本是括号下面最高的优先级了
            p7exp->I9RightExp = parseExpression(NOT_SUB_DEC_INC_P);
            return p7exp;
        }

        //字符串值
        StringExpression *parseStringExpression() {
            auto *e = new StringExpression(nowToken);
            getNextToken();
            return e;
        };

        //左括号（算术运算）
        I9Expression *parseLParenExpression() {
            //now='(',next=表达式
            getNextToken(); //跳过'('
            auto *p7exp = parseExpression(LOWEST_P);
            //now=')',next=后面的表达式
            if (!expectNowTokenIs(RPAREN)) {
                return nullptr;
            }
            getNextToken(); //跳过')'
            return p7exp;
        }

        //null值
        NullExpression *parseNullExpression() {
            auto *e = new NullExpression(nowToken);
            getNextToken();
            return e;
        };

        //布尔值
        BoolExpression *parseBoolExpression() {
            auto *e = new BoolExpression(nowToken);
            getNextToken();
            return e;
        };

        //整数值
        IntExpression *parseIntExpression() {
            auto *e = new IntExpression(nowToken);
            getNextToken();
            return e;
        };

        //浮点数值
        FloatExpression *parseFloatExpression() {
            auto *e = new FloatExpression(nowToken);
            getNextToken();
            return e;
        };

        //if语句
        IfExpression *parseIfExpression() {
            //now=if,next='('
            auto *p7exp = new IfExpression(nowToken);
            if (!expectNextTokenIs(LPAREN)) {
                return nullptr;
            }
            getNextToken(); //跳过'('
            //now='(',next=条件表达式
            p7exp->I9ConditionExp = parseExpression(LOWEST_P);
            //now=')',next='{'
            if (!expectNowTokenIs(RPAREN)) {
                return nullptr;
            }
            if (!expectNextTokenIs(LBRACE)) {
                return nullptr;
            }
            //now='{',next=块语句
            p7exp->P7ConsequenceStmt = parseBlockStatement();
            //now=else,next='{'或者没有
            if (nowTokenIs(ELSE_HKNI)) {
                getNextToken(); //跳过else
                //now='{',next=块语句
                if (!expectNowTokenIs(LBRACE)) {
                    return nullptr;
                }
                p7exp->P7AlternativeStmt = parseBlockStatement();
            } else {
                p7exp->P7AlternativeStmt = nullptr;
            }
            return p7exp;
        }

        //for循环语句
        ForExpression *parseForExpression() {
            auto *p7exp = new ForExpression(nowToken);
            if (!expectNextTokenIs(LPAREN)) {
                return nullptr;
            }
            getNextToken();
            p7exp->I9InitStmt = parseStatement();
            if (!nowTokenIs(SEMICOLON)) {
                reportError("parseForExpression error,TokenHKNI=" + nextToken.Literal + "。");
                return nullptr;
            }
            getNextToken();
            p7exp->I9ConditionStmt = parseStatement();
            if (!nowTokenIs(SEMICOLON)) {
                reportError("parseForExpression error,TokenHKNI=" + nextToken.Literal + "。");
                return nullptr;
            }
            getNextToken();
            p7exp->I9IncrementStmt = parseStatement();
            if (!expectNextTokenIs(RPAREN)) {
                return nullptr;
            }
            if (!expectNextTokenIs(LBRACE)) {
                return nullptr;
            }
            return p7exp;
        }

        //函数定义
        FuncExpression *parseFuncExpression() {
            //now=关键字,next=函数名
            auto *p7exp = new FuncExpression(nowToken);
            if (nextTokenIs(IDENTIFIER)) {
                getNextToken();
                p7exp->P7NameExp = new IdentifierExpression(nowToken); //处理函数名
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
            p7exp->P7BodyBlockStmt = parseBlockStatement(); //处理函数体
            return p7exp;
        }

        //函数定义的参数列表
        void parseFuncArgsList(FuncExpression *p7exp) {
            //无参数
            if (nowTokenIs(RPAREN)) {
                return;
            }
            //有参数
            std::vector<IdentifierExpression *> argList;
            std::vector<TokenType> argTypeList;

            //第一个参数
            argList.push_back(new IdentifierExpression(nowToken)); //标识符
            this->getNextToken();
            argTypeList.push_back(nowToken.TokenType); //参数类型
            this->getNextToken();

            while (nowTokenIs(COMMA)) {
                this->getNextToken(); //跳过','

                //下一个参数
                argList.push_back(new IdentifierExpression(nowToken));
                this->getNextToken();
                argTypeList.push_back(nowToken.TokenType);
                this->getNextToken();
            }

            p7exp->P7ArgTypeList = argTypeList;
            p7exp->P7ArgList = argList;
        }

        //解析标识符
        IdentifierExpression *parseIdentifierExpression() {
            auto *e = new IdentifierExpression(nowToken);
            getNextToken();
            return e;
        }

        //##前缀##

        //##中缀

        //中缀表达式，通用方法
        InfixExpression *parseInfixExpression(I9Expression *i9exp) {
            auto *p7exp = new InfixExpression(nowToken);
            p7exp->I9LeftExp = i9exp;
            int p = this->getNowTokenPrecedence(); //获取中缀标记的优先级
            getNextToken(); //跳过中缀符号
            p7exp->I9RightExp = parseExpression(p); //这里传的是中缀符号的优先级
            return p7exp;
        }

        //赋值
        AssignExpression *parseAssignExpression(I9Expression *i9exp) {
            auto *p7exp = new AssignExpression(nowToken);
            p7exp->I9NameExp = dynamic_cast<IdentifierExpression *>(i9exp);
            getNextToken();
            p7exp->I9ValueExp = parseExpression(LOWEST_P);
            return p7exp;
        }

        //左括号（函数调用）
        CallExpression *parseCallExpression(I9Expression *funcEXP) {
            //now='(',next=参数列表或者')'
            auto *p7exp = new CallExpression(nowToken);
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
        void parseCallArgsList(CallExpression *p7exp) {
            //无参数
            if (nowTokenIs(RPAREN)) {
                return;
            }
            //有参数
            std::vector<I9Expression *> argList;

            argList.push_back(parseExpression(LOWEST_P));
            while (nowTokenIs(COMMA)) {
                this->getNextToken(); //跳到','
                argList.push_back(parseExpression(LOWEST_P));
            }
            p7exp->ArgExpList = argList;
        }

        //##中缀##

        //##解析表达式##

        //##解析语句

        //解析语句，递归入口
        I9Statement *parseStatement() {
            switch (nowToken.TokenType) {
                case VAR_HKNI:
                    return parseVarStatement();
                case RETURN_HKNI:
                    return parseReturnStatement();
                default:
                    return parseExpressionStatement();
            }
        }

        VarStatement *parseVarStatement() {
            //now=var,next=标识符
            auto *p7stmt = new VarStatement();
            if (!expectNextTokenIs(IDENTIFIER)) {
                return nullptr;
            }
            //now=标识符,next=变量类型
            p7stmt->P7IdentifierExp = new IdentifierExpression(nowToken);
            if (!this->nextTokenIs(BOOL_TYPE) &&
                !this->nextTokenIs(INT_TYPE) &&
                !this->nextTokenIs(FLOAT_TYPE) &&
                !this->nextTokenIs(STRING_TYPE)) {
                reportError("变量声明语句解析错误，TokenHKNI=" + nextToken.Literal + "。");
                return nullptr;
            }
            this->getNextToken();
            //now=变量类型,next=';'或者'='
            p7stmt->TokenHKNI = nowToken;
            p7stmt->ValueType = nowToken.TokenType;
            this->getNextToken();
            //now=';'或者'=',next=
            if (nowTokenIs(SEMICOLON)) {
                this->getNextToken(); //跳过';'
                return p7stmt; //var 标识符 类型;
            }
            //now='=',next=变量值
            if (!expectNowTokenIs(ASSIGN)) {
                return nullptr;
            }
            this->getNextToken(); //跳过'='
            //now=变量值,next=';'
            p7stmt->I9ValueExp = parseExpression(LOWEST_P);
            if (!expectNowTokenIs(SEMICOLON)) {
                return nullptr;
            }
            this->getNextToken(); //跳过';'
            return p7stmt; //var 标识符 类型 = 值;
        }

        ReturnStatement *parseReturnStatement() {
            //now=return,next=';'或者表达式
            auto *p7stmt = new ReturnStatement(nowToken);
            this->getNextToken(); //跳过return
            if (nowTokenIs(SEMICOLON)) {
                getNextToken(); //跳过';'
                return p7stmt;
            }
            p7stmt->I9Exp = parseExpression(LOWEST_P);
            //now=';',next=
            if (!expectNowTokenIs(SEMICOLON)) {
                return nullptr;
            }
            getNextToken(); //跳过';'
            return p7stmt;
        }

        ExpressionStatement *parseExpressionStatement() {
            auto *p7stmt = new ExpressionStatement(nowToken);
            p7stmt->I9Exp = parseExpression(LOWEST_P);
            if (nowTokenIs(SEMICOLON)) {
                getNextToken(); //跳过';'
            }
            return p7stmt;
        }

        //块语句，会处理首尾的'{'和'}'
        BlockStatement *parseBlockStatement() {
            //now='{',next=
            auto *p7stmt = new BlockStatement(nowToken);
            getNextToken(); //跳过'{'
            while (!nowTokenIs(RBRACE)) {
                //跳过注释
                if (nowTokenIs(COMMENT)) {
                    getNextToken();
                    continue;
                }
                //解析语句
                auto *i9stmt = parseStatement();
                if (i9stmt != nullptr) {
                    p7stmt->BodyStmtList.push_back(i9stmt);
                }
            }
            //now='}',next=
            getNextToken(); //跳过'}'
            return p7stmt;
        }

        //##解析语句##
    };

}

#endif
