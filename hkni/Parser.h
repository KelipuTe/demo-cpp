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

            while (nowToken.TokenType != END) {
                if (nowToken.TokenType == ILLEGAL) {
                    string modeStr = this->p7lexer->GetModeStr();
                    string rowStr = std::to_string(this->p7lexer->GetNowRow());
                    string columnStr = std::to_string(this->p7lexer->GetNowColumn());
                    string t4str = modeStr + rowStr + "行；" + columnStr + "列；DoParse；语法错误：" + nowToken.Literal;
                    errorList.push_back(t4str);
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
        //初始化词法标记优先级。这里不需要用find函数判断key是否存在。
        //如果在map里没找到，会返回默认值0，0就是LOWEST_P，逻辑上不会出问题。
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
            f8PrefixParsingMap[LPAREN] = std::bind(&Parser::parseLParenExpression, this); //左括号，算数运算
            //关键字
            f8PrefixParsingMap[NULL_HKNI] = std::bind(&Parser::parseNullExpression, this); //null值
            f8PrefixParsingMap[INT_VALUE] = std::bind(&Parser::parseIntExpression, this); //整数值
            f8PrefixParsingMap[FLOAT_VALUE] = std::bind(&Parser::parseFloatExpression, this); //浮点数值
            //其他
            f8PrefixParsingMap[IDENTIFIER] = std::bind(&Parser::parseIdentifierExpression, this); //标识符

            //中缀
            //运算符
            f8InfixParsingMap[NEQ] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //不等于
            f8InfixParsingMap[MOD] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //取模
            f8InfixParsingMap[MOD_ASSIGN] = std::bind(&Parser::parseAssignExpression, this,
                                                      std::placeholders::_1); //取模赋值
            f8InfixParsingMap[BIT_AND] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //位与
            f8InfixParsingMap[AND] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //逻辑与
            f8InfixParsingMap[MUL] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //乘法
            f8InfixParsingMap[MUL_ASSIGN] = std::bind(&Parser::parseAssignExpression, this,
                                                      std::placeholders::_1); //乘赋值
            f8InfixParsingMap[SUB] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //减法
            f8InfixParsingMap[SUB_ASSIGN] = std::bind(&Parser::parseAssignExpression, this,
                                                      std::placeholders::_1); //减赋值
            f8InfixParsingMap[ASSIGN] = std::bind(&Parser::parseAssignExpression, this, std::placeholders::_1); //赋值
            f8InfixParsingMap[EQ] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //等于
            f8InfixParsingMap[ADD] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //加法
            f8InfixParsingMap[ADD_ASSIGN] = std::bind(&Parser::parseAssignExpression, this,
                                                      std::placeholders::_1); //加赋值
            f8InfixParsingMap[LPAREN] = std::bind(&Parser::parseCallExpression, this, std::placeholders::_1); //左括号，函数调用
            f8InfixParsingMap[BIT_OR] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //位或
            f8InfixParsingMap[OR] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //逻辑或
            f8InfixParsingMap[LT] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //小于
            f8InfixParsingMap[LTE] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //小于等于
            f8InfixParsingMap[GT] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //大于
            f8InfixParsingMap[GTE] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //大于等于
            f8InfixParsingMap[DIV] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //除法
            f8InfixParsingMap[DIV_ASSIGN] = std::bind(&Parser::parseAssignExpression, this,
                                                      std::placeholders::_1); //除赋值
            //关键字
            f8PrefixParsingMap[BOOL_VALUE] = std::bind(&Parser::parseBoolExpression, this); //布尔值
            f8PrefixParsingMap[IF_HKNI] = std::bind(&Parser::parseIfExpression, this); //if语句
            f8PrefixParsingMap[FOR_HKNI] = std::bind(&Parser::parseForExpression, this); //for循环语句
            f8PrefixParsingMap[FUNCTION_HKNI] = std::bind(&Parser::parseFuncExpression, this); //函数定义
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

        void reportError(string msg) {
            string modeStr = this->p7lexer->GetModeStr();
            string rowStr = std::to_string(this->p7lexer->GetNowRow());
            string columnStr = std::to_string(this->p7lexer->GetNowColumn());
            string str = modeStr + rowStr + "行；" + columnStr + "列。语法错误：" + msg + "。";
            errorList.push_back(str);
        }

        //期望当前token是指定type类型的。
        //如果是就返回true并；如果不是就返回false并报错；
        bool expectNowTokenIs(TokenType type) {
            if (nowTokenIs(type)) {
                return true;
            } else {
                reportError("expectNowTokenIs error,given " + nextToken.Literal);
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
                reportError("expectNextTokenIs error,given " + nextToken.Literal);
                return false;
            }
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
            auto f8PrefixParsing = f8PrefixParsingMap[nowToken.TokenType];
            if (f8PrefixParsing == nullptr) {
                string modeStr = this->p7lexer->GetModeStr();
                string rowStr = std::to_string(this->p7lexer->GetNowRow());
                string columnStr = std::to_string(this->p7lexer->GetNowColumn());
                string t4str = modeStr + rowStr + "行；" + columnStr + "列；f8PrefixParsing；语法错误：" + nowToken.Literal;
                errorList.push_back(t4str);
                return nullptr;
            }
            auto leftExp = f8PrefixParsing();

            while (!nextTokenIs(SEMICOLON) && precedence < getNextTokenPrecedence()) {
                auto f8InfixParsing = f8InfixParsingMap[nextToken.TokenType];
                if (f8InfixParsing == nullptr) {
                    string modeStr = this->p7lexer->GetModeStr();
                    string rowStr = std::to_string(this->p7lexer->GetNowRow());
                    string columnStr = std::to_string(this->p7lexer->GetNowColumn());
                    string t4str =modeStr + rowStr + "行；" + columnStr + "列；f8InfixParsing；语法错误：" + nowToken.Literal;
                    errorList.push_back(t4str);
                    return leftExp;
                }
                getNextToken();
                leftExp = f8InfixParsing(leftExp);
            }

            return leftExp;
        }

        //解析标识符
        IdentifierExpression *parseIdentifierExpression() {
            return new IdentifierExpression(nowToken);
        }

        NullExpression *parseNullExpression() {
            return new NullExpression(nowToken);
        };

        IntExpression *parseIntExpression() {
            return new IntExpression(nowToken);
        };

        FloatExpression *parseFloatExpression() {
            return new FloatExpression(nowToken);
        };

        StringExpression *parseStringExpression() {
            return new StringExpression(nowToken);
        };

        I9Expression *parseLParenExpression() {
            getNextToken(); //跳过'('标记
            auto *p7exp = parseExpression(LOWEST_P);
            if (!expectNextTokenIs(RPAREN)) {
                return nullptr;
            }
            return p7exp;
        }

        IfExpression *parseIfExpression() {
            IfExpression *p7exp = new IfExpression(nowToken);

            //if(条件表达式)
            if (!expectNextTokenIs(LPAREN)) {
                return nullptr;
            }
            getNextToken(); //跳过'('标记
            p7exp->I9ConditionExp = parseExpression(LOWEST_P);
            if (!expectNextTokenIs(RPAREN)) {
                return nullptr;
            }

            //{块语句}
            if (!expectNextTokenIs(LBRACE)) {
                return nullptr;
            }
            p7exp->P7ConsequenceStmt = parseBlockStatement();

            //else
            if (nextTokenIs(ELSE_HKNI)) {
                getNextToken(); //跳过"else"标记
                //{块语句}
                if (!expectNextTokenIs(LBRACE)) {
                    return nullptr;
                }
                p7exp->P7AlternativeStmt = parseBlockStatement();
            } else {
                p7exp->P7AlternativeStmt = nullptr;
            }

            return p7exp;
        }

        ForExpression *parseForExpression() {
            ForExpression *p7exp = new ForExpression(nowToken);

            if (!expectNextTokenIs(LPAREN)) {
                return nullptr;
            }
            getNextToken();
            p7exp->I9InitStmt = parseStatement();
            if (!nowTokenIs(SEMICOLON)) {
                reportError("parseForExpression error,given " + nextToken.Literal);
                return nullptr;
            }
            getNextToken();
            p7exp->I9ConditionStmt = parseStatement();
            if (!nowTokenIs(SEMICOLON)) {
                reportError("parseForExpression error,given " + nextToken.Literal);
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

        FuncExpression *parseFuncExpression() {
            FuncExpression *p7exp = new FuncExpression(nowToken);

            //now=关键字,next=函数名
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
            if (!expectNowTokenIs(RPAREN)) {
                return nullptr;
            }
            //now=')',next=返回值类型或者'{'
            if (nextTokenIs(BOOL_TYPE) ||
                nextTokenIs(INT_TYPE) ||
                nextTokenIs(FLOAT_TYPE) ||
                nextTokenIs(STRING_TYPE)) {
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

        CallExpression *parseCallExpression(I9Expression *funcEXP) {
            CallExpression *p7exp = new CallExpression(nowToken);
            p7exp->I9Exp = funcEXP;
            parseCallArgsList(p7exp);

            return p7exp;
        }

        void parseCallArgsList(CallExpression *p7exp) {
            //无参数
            if (nextTokenIs(RPAREN)) {
                this->getNextToken();
                return;
            }

            //有参数
            std::vector<I9Expression *> argList;
            this->getNextToken();
            argList.push_back(parseExpression(LOWEST_P));
            while (nextTokenIs(COMMA)) {
                this->getNextToken(); //跳到','
                this->getNextToken(); //跳过','
                argList.push_back(parseExpression(LOWEST_P));
            }

            expectNextTokenIs(RPAREN);

            p7exp->ArgExpList = argList;
        }

        AssignExpression *parseAssignExpression(I9Expression *i9exp) {
            auto *p7exp = new AssignExpression(nowToken);
            p7exp->I9NameExp = dynamic_cast<IdentifierExpression *>(i9exp);
            getNextToken();
            p7exp->I9ValueExp = parseExpression(LOWEST_P);
            return p7exp;
        }

        PrefixExpression *parsePrefixExpression() {
            auto *p7exp = new PrefixExpression(nowToken);
            getNextToken(); //跳过前缀标记
            //这几个前缀符号的优先级基本是括号下面最高的优先级了
            p7exp->I9RightExp = parseExpression(NOT_SUB_DEC_INC_P);
            return p7exp;
        }

        InfixExpression *parseInfixExpression(I9Expression *i9exp) {
            auto *p7exp = new InfixExpression(nowToken);
            p7exp->I9LeftExp = i9exp;
            int precedence = this->getNowTokenPrecedence(); //获取中缀标记的优先级
            getNextToken(); //跳过中缀标记
            p7exp->I9RightExp = parseExpression(precedence); //这里传的是中缀标记的优先级
            return p7exp;
        }

        BoolExpression *parseBoolExpression() {
            return new BoolExpression(nowToken);
        };

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
            auto *p7stmt = new VarStatement();

            if (!expectNextTokenIs(IDENTIFIER)) {
                return nullptr;
            }
            p7stmt->P7IdentifierExp = new IdentifierExpression(nowToken);

            if (!this->nextTokenIs(BOOL_TYPE) &&
                !this->nextTokenIs(INT_TYPE) &&
                !this->nextTokenIs(FLOAT_TYPE) &&
                !this->nextTokenIs(STRING_TYPE)) {
                reportError("parseVarStatement error,given " + nextToken.Literal);
                return nullptr;
            }
            this->getNextToken();
            p7stmt->SetToken(nowToken);

            if (this->nextTokenIs(SEMICOLON)) {
                this->getNextToken();
                return p7stmt; //var 标识符 类型;
            }

            if (!expectNextTokenIs(ASSIGN)) {
                return nullptr;
            }
            this->getNextToken();
            p7stmt->I9ValueExp = parseExpression(LOWEST_P);
            if (this->nextTokenIs(SEMICOLON)) {
                this->getNextToken();
            }
            getNextToken();
            return p7stmt; //var 标识符 类型 = 值;
        }

        ReturnStatement *parseReturnStatement() {
            auto *p7stmt = new ReturnStatement(nowToken);
            this->getNextToken();
            p7stmt->I9Exp = parseExpression(LOWEST_P);
            if (this->nextTokenIs(SEMICOLON)) {
                this->getNextToken();
            }
            getNextToken();
            return p7stmt;
        }

        ExpressionStatement *parseExpressionStatement() {
            auto *p7stmt = new ExpressionStatement(nowToken);
            p7stmt->I9Exp = parseExpression(LOWEST_P);
            if (nextTokenIs(SEMICOLON)) {
                getNextToken(); //跳过';'
            }
            getNextToken();
            return p7stmt;
        }

        BlockStatement *parseBlockStatement() {
            BlockStatement *p7stmt = new BlockStatement(nowToken);

            getNextToken(); //跳过'{'

            while (!nowTokenIs(RBRACE)) {
                //跳过注释
                if (nowTokenIs(COMMENT)) {
                    getNextToken();
                    continue;
                }

                auto *i9stmt = parseStatement();
                if (i9stmt != nullptr) {
                    p7stmt->BodyStmtList.push_back(i9stmt);
                }
            }

            return p7stmt;
        }

        //##解析语句##
    };

}

#endif
