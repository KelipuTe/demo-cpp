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
#include "expression/StructExpression.h"
#include "expression/TypeExpression.h"

#include "statement/ExpressionStatement.h"
#include "statement/ReturnStatement.h"
#include "statement/VarStatement.h"

#include "ast/Program.h"

using namespace asthkni;

namespace hkni {
    //语法分析器，基于运算符优先级和递归的实现。
    class Parser {
    private:
        //类型别名
        using prefixParsingF8 = std::function<ExpressionI9 * ()>; //前缀标记的解析函数
        using infixParsingF8 = std::function<ExpressionI9 * (ExpressionI9 * )>; //中缀标记的解析函数

        Lexer *lexerP7; //词法分析器

        Token nowToken; //当前token
        Token nextToken; //下一个token

        std::map<TokenType, PRECEDENCE> precedenceMap; //词法标记的优先级
        //如果在map里没找到，会返回类型的默认值0，0就是LOWEST_P，逻辑上不会出问题。

        std::map<TokenType, prefixParsingF8> prefixParsingF8Map; //前缀词法标记的解析函数
        std::map<TokenType, infixParsingF8> infixParsingF8Map; //中缀词法标记的解析函数
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

        //语法分析入口
        Program *DoParse() {
            auto *programP7 = new Program();

            int i = 0;
            while (nowToken.TokenType != END) {
                if (nowToken.TokenType == ILLEGAL) {
                    reportError("语法错误：" + nowToken.Literal + "。");
                    free(programP7);
                    return nullptr;
                }
                if (nowToken.TokenType == COMMENT) {
                    //遇到注释直接跳过
                    getNextToken();
                } else {
                    //解析语句，一个程序由多条语句组成
                    StatementI9 *stmtI9 = this->parseStatement();
                    if (stmtI9 != nullptr) {
                        programP7->StmtI9List.push_back(stmtI9);
                    }
                }
                i++;
                if (i > 10000) {
                    reportError("程序太长或者程序异常导致死循环。");
                    break;
                }
            }

            return programP7;
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
        //写代码的顺序和TokenType那里的顺序一样，方便找。
        void initParsingFunction() {
            //前缀

            //运算符
            prefixParsingF8Map[STRING_VALUE] = std::bind(&Parser::parseStringExpression, this); //字符串值
            prefixParsingF8Map[NOT] = std::bind(&Parser::parsePrefixExpression, this); //逻辑非
            prefixParsingF8Map[SUB] = std::bind(&Parser::parsePrefixExpression, this); //负号
            prefixParsingF8Map[DEC] = std::bind(&Parser::parsePrefixExpression, this); //自减
            prefixParsingF8Map[INC] = std::bind(&Parser::parsePrefixExpression, this); //自增
            prefixParsingF8Map[LPAREN] = std::bind(&Parser::parseLParenExpression, this); //左括号（算术运算）

            //关键字
            prefixParsingF8Map[NULL_HKNI] = std::bind(&Parser::parseNullExpression, this); //null值
            prefixParsingF8Map[BOOL_VALUE] = std::bind(&Parser::parseBoolExpression, this); //布尔值
            prefixParsingF8Map[INT_VALUE] = std::bind(&Parser::parseIntExpression, this); //整数值
            prefixParsingF8Map[FLOAT_VALUE] = std::bind(&Parser::parseFloatExpression, this); //浮点数值
            prefixParsingF8Map[IF_HKNI] = std::bind(&Parser::parseIfExpression, this); //if语句
            prefixParsingF8Map[FOR_HKNI] = std::bind(&Parser::parseForExpression, this); //for循环语句
            prefixParsingF8Map[FUNCTION_HKNI] = std::bind(&Parser::parseFuncExpression, this); //函数定义
            prefixParsingF8Map[TYPE_HKNI] = std::bind(&Parser::parseTypeExpression, this); //类型定义

            //其他
            prefixParsingF8Map[IDENTIFIER] = std::bind(&Parser::parseIdentifierExpression, this); //标识符

            //中缀

            //运算符
            infixParsingF8Map[NEQ] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //不等于
            infixParsingF8Map[MOD] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //取模
            infixParsingF8Map[MOD_ASSIGN] =
                    std::bind(&Parser::parseAssignExpression, this, std::placeholders::_1); //取模赋值
            infixParsingF8Map[BIT_AND] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //位与
            infixParsingF8Map[AND] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //逻辑与
            infixParsingF8Map[MUL] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //乘法
            infixParsingF8Map[MUL_ASSIGN] =
                    std::bind(&Parser::parseAssignExpression, this, std::placeholders::_1); //乘赋值
            infixParsingF8Map[SUB] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //减法
            infixParsingF8Map[SUB_ASSIGN] =
                    std::bind(&Parser::parseAssignExpression, this, std::placeholders::_1); //减赋值
            infixParsingF8Map[ASSIGN] = std::bind(&Parser::parseAssignExpression, this, std::placeholders::_1); //赋值
            infixParsingF8Map[EQ] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //等于
            infixParsingF8Map[ADD] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //加法
            infixParsingF8Map[ADD_ASSIGN] =
                    std::bind(&Parser::parseAssignExpression, this, std::placeholders::_1); //加赋值
            infixParsingF8Map[LPAREN] =
                    std::bind(&Parser::parseCallExpression, this, std::placeholders::_1); //左括号（函数调用）
            infixParsingF8Map[BIT_OR] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //位或
            infixParsingF8Map[OR] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //逻辑或
            infixParsingF8Map[LT] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //小于
            infixParsingF8Map[LTE] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //小于等于
            infixParsingF8Map[GT] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //大于
            infixParsingF8Map[GTE] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //大于等于
            infixParsingF8Map[DIV] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1); //除法
            infixParsingF8Map[DIV_ASSIGN] =
                    std::bind(&Parser::parseAssignExpression, this, std::placeholders::_1); //除赋值
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
            string modeStr = lexerP7->GetModeStr();
            string rowStr = std::to_string(lexerP7->GetNowRow());
            string columnStr = std::to_string(lexerP7->GetNowColumn());
            string errStr = modeStr + rowStr + "行；" + columnStr + "列。" + msg;
            errorList.push_back(errStr);
        }

        int getNowTokenPrecedence() {
            return precedenceMap[nowToken.TokenType];
        }

        int getNextTokenPrecedence() {
            return precedenceMap[nextToken.TokenType];
        }

        //##解析语句的代码

        //解析语句
        StatementI9 *parseStatement() {
            switch (nowToken.TokenType) {
                case VAR_HKNI:
                    return parseVarStatement();
                case RETURN_HKNI:
                    return parseReturnStatement();
                default:
                    return parseExpressionStatement();
            }
        }

        //var语句
        VarStatement *parseVarStatement() {
            //now=var,next=标识符
            auto *p7stmt = new VarStatement();
            if (!expectNextTokenIs(IDENTIFIER)) {
                free(p7stmt);
                return nullptr;
            }
            //now=标识符,next=变量类型
            p7stmt->P7IdentifierExp = new IdentifierExpression(nowToken);
            if (!this->nextTokenIs(BOOL_TYPE) &&
                !this->nextTokenIs(INT_TYPE) &&
                !this->nextTokenIs(FLOAT_TYPE) &&
                !this->nextTokenIs(STRING_TYPE)) {
                reportError("变量声明语句解析错误，nextTokenIs=" + nextToken.Literal + "。");
                free(p7stmt);
                return nullptr;
            }
            this->getNextToken();
            //now=变量类型,next=';'或者'='
            p7stmt->ValueToken = nowToken;
            p7stmt->ValueTokenType = nowToken.TokenType;
            this->getNextToken();
            //now=';'或者'=',next=
            if (nowTokenIs(SEMICOLON)) {
                this->getNextToken(); //跳过';'
                return p7stmt; //var 标识符 类型;
            }
            //now='=',next=变量值
            if (!expectNowTokenIs(ASSIGN)) {
                free(p7stmt);
                return nullptr;
            }
            this->getNextToken(); //跳过'='
            //now=变量值,next=';'
            p7stmt->I9ValueExp = parseExpression(LOWEST_P);
            if (!expectNowTokenIs(SEMICOLON)) {
                free(p7stmt);
                return nullptr;
            }
            this->getNextToken(); //跳过';'
            return p7stmt; //var 标识符 类型 = 值;
        }

        //return语句
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
                free(p7stmt);
                return nullptr;
            }
            getNextToken(); //跳过';'
            return p7stmt;
        }

        //表达式语句
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
            auto *p7stmt = new BlockStatement();
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

        //##解析语句的代码##

        //##解析表达式的代码

        //解析表达式
        //运算符的解析方法可能会递归调用该函数。
        ExpressionI9 *parseExpression(int precedence) {
            //前缀解析方法
            auto f8Prefix = prefixParsingF8Map[nowToken.TokenType];
            if (f8Prefix == nullptr) {
                reportError("未找到 " + nowToken.Literal + " 的前缀解析方法。");
                return nullptr;
            }
            auto leftExp = f8Prefix(); //调用前缀解析方法

            //如果当前token不是分号，而且当前的符号优先级比之前的符号优先级高，则优先按中缀解析
            while (!nowTokenIs(SEMICOLON) && precedence < getNowTokenPrecedence()) {
                //中缀解析方法
                auto f8Infix = infixParsingF8Map[nowToken.TokenType];
                if (f8Infix == nullptr) {
                    reportError("未找到 " + nowToken.Literal + " 的中缀解析方法。");
                    return nullptr;
                }
                leftExp = f8Infix(leftExp);//调用中缀解析方法
            }

            return leftExp;
        }

        //##处理前缀结构的代码

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
        ExpressionI9 *parseLParenExpression() {
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
            auto *p7exp = new IfExpression();
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
            //now=for,next='('
            auto *p7exp = new ForExpression();
            if (!expectNextTokenIs(LPAREN)) {
                return nullptr;
            }
            //now='(',next=初始化语句
            getNextToken();
            //now=初始化语句,next
            if (nowTokenIs(SEMICOLON)) {
                //初始化语句为空
            } else {
                p7exp->I9InitExp = parseExpression(LOWEST_P);
            }
            getNextToken(); //跳过分号
            //now=条件表达式,next=
            if (nowTokenIs(SEMICOLON)) {
                //条件表达式为空
            } else {
                p7exp->I9ConditionExp = parseExpression(LOWEST_P);
            }
            getNextToken(); //跳过分号
            //now=递增递减语句,next=
            if (nowTokenIs(RPAREN)) {
                //递增递减语句为空，什么都不做
            } else {
                p7exp->I9IncrementExp = parseExpression(LOWEST_P);
            }
            //now=')',next='{'
            if (!expectNowTokenIs(RPAREN)) {
                return nullptr;
            }
            if (!expectNextTokenIs(LBRACE)) {
                return nullptr;
            }
            //now='{',next=块语句
            p7exp->P7BodyStmt = parseBlockStatement();

            return p7exp;
        }

        //函数定义
        FuncExpression *parseFuncExpression() {
            //now=关键字,next=函数名
            auto *p7exp = new FuncExpression();
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

        TypeExpression *parseTypeExpression() {
            //now="type"，next=类型标识符

            auto *typeP7 = new TypeExpression();

            if (!expectNextTokenIs(IDENTIFIER)) {
                return nullptr;
            } //now=类型标识符，next=类型关键字

            auto *nameP7 = new IdentifierExpression(nowToken); //处理标识符

            this->getNextToken(); //now=类型关键字，next='{'

            if (!nowTokenIs(STRUCT_HKNI)) {
                reportError("类型定义语句解析错误，nowTokenIs=" + nowToken.Literal + "。");
                return nullptr;
            }

            if (nowTokenIs(STRUCT_HKNI)) {
                //struct
                auto structP7 = new StructExpression();
                structP7->NameP7 = nameP7;

                if (!expectNextTokenIs(LBRACE)) {
                    return nullptr;
                } //now='{'，next=字段标识符或者'}'

                this->getNextToken(); //跳过'{'，now=字段标识符或者'}'，next=

                parseStructField(structP7); //解析完，now='}'，next=

                if (!expectNowTokenIs(RPAREN)) {
                    return nullptr;
                }

                typeP7->RealType = nowToken.TokenType;
                typeP7->RealExpP7 = structP7;
            }

            return typeP7;
        }

        //解析结构体字段
        void parseStructField(StructExpression *structP7) {
            //无字段，now='}'，next=
            if (nowTokenIs(RPAREN)) {
                return;
            }

            //有字段，now=字段标识符，next=字段类型
            std::map<string, StructField *> fieldP7Map;

            StructField *field;
            while (true) {
                field = new StructField();
                field->FieldP7 = new IdentifierExpression(nowToken); //字段标识符

                this->getNextToken(); //now=字段类型，next=','

                field->FieldType = nowToken.TokenType; //字段类型
                fieldP7Map[field->FieldP7->GetTokenLiteral()] = field;

                this->getNextToken(); //now=','，next=字段标识符或者'}'

                if (nowTokenIs(COMMA)) {
                    continue;
                }

                break;
            }

            structP7->FieldP7Map = fieldP7Map;
        }

        //解析标识符
        IdentifierExpression *parseIdentifierExpression() {
            auto *e = new IdentifierExpression(nowToken);
            getNextToken();
            return e;
        }

        //##处理前缀结构的代码##

        //##处理中缀结构的代码

        //中缀表达式，通用方法
        InfixExpression *parseInfixExpression(ExpressionI9 *i9exp) {
            auto *p7exp = new InfixExpression(nowToken);
            p7exp->I9LeftExp = i9exp;
            int p = this->getNowTokenPrecedence(); //获取中缀标记的优先级
            getNextToken(); //跳过中缀符号
            p7exp->I9RightExp = parseExpression(p); //这里传的是中缀符号的优先级
            return p7exp;
        }

        //赋值
        AssignExpression *parseAssignExpression(ExpressionI9 *i9exp) {
            auto *p7exp = new AssignExpression(nowToken);
            p7exp->I9NameExp = dynamic_cast<IdentifierExpression *>(i9exp);
            getNextToken();
            p7exp->I9ValueExp = parseExpression(LOWEST_P);
            return p7exp;
        }

        //左括号（函数调用）
        CallExpression *parseCallExpression(ExpressionI9 *funcEXP) {
            //now='(',next=参数列表或者')'
            auto *p7exp = new CallExpression();
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
            std::vector<ExpressionI9 *> argList;

            argList.push_back(parseExpression(LOWEST_P));
            while (nowTokenIs(COMMA)) {
                this->getNextToken(); //跳到','
                argList.push_back(parseExpression(LOWEST_P));
            }
            p7exp->ArgExpList = argList;
        }

        //##处理中缀结构的代码##

        //##解析表达式的代码##
    };

}

#endif
