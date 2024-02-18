#ifndef HKNI_PARSER_H
#define HKNI_PARSER_H

#include <map>
#include <functional>

#include "Lexer.h"

#include "Precedence.h"

#include "exp/IdentifierExpression.h"

#include "exp/NullExpression.h"
#include "exp/IntExpression.h"
#include "exp/FloatExpression.h"
#include "exp/StringExpression.h"

#include "exp/AssignExpression.h"

#include "exp/PrefixExpression.h"
#include "exp/InfixExpression.h"
#include "exp/SuffixExpression.h"

#include "exp/BoolExpression.h"
#include "exp/IfExpression.h"
#include "exp/ForExpression.h"

#include "exp/FuncExpression.h"
#include "exp/CallExpression.h"

#include "stmt/VarStatement.h"
#include "stmt/ReturnStatement.h"
#include "stmt/ExpressionStatement.h"

#include "ast/Program.h"

using namespace ast;

//语法分析器，基于运算符优先级和递归的实现。
class Parser {
    //####属性
private:
    //类型别名
    using f8PrefixParsing = std::function<I9Expression *()>;
    using f8InfixParsing = std::function<I9Expression *(I9Expression *)>;

    Lexer *p7lexer; //词法分析器

    Token nowToken; //当前token
    Token nextToken; //下一个token

    std::map<TOKEN_TYPE, PRECEDENCE> precedenceMap; //词法标记的优先级
    std::map<TOKEN_TYPE, f8PrefixParsing> f8PrefixParsingMap; //前缀词法标记的解析方法
    std::map<TOKEN_TYPE, f8InfixParsing> f8InfixParsingMap; //中缀词法标记的解析方法

    vector<string> errorList; //错误列表
    //####方法
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
            } else if (nowToken.TokenType == COMMENT) {

            } else {
                I9Statement *i9stmt = this->parseStatement();
                if (i9stmt != nullptr) {
                    p7program->I9StatementList.push_back(i9stmt);
                }
            }
            getNextToken();
        }

        return p7program;
    };
private:
    //初始化词法标记优先级。如果在map里没找到，会返回默认值0，LOWEST_P就是0。
    void initPrecedence() {
        precedenceMap[ASSIGN] = ASSIGN_P;

        precedenceMap[ADD] = ADD_SUB_P;
        precedenceMap[SUB] = ADD_SUB_P;
        precedenceMap[MUL] = MUL_DIV_MOD_P;
        precedenceMap[DIV] = MUL_DIV_MOD_P;
        precedenceMap[MOD] = MUL_DIV_MOD_P;

        precedenceMap[INC] = SUB_INC_DEC_NOT_P;
        precedenceMap[DEC] = SUB_INC_DEC_NOT_P;

        precedenceMap[ADD_ASSIGN] = ASSIGN_P;
        precedenceMap[SUB_ASSIGN] = ASSIGN_P;
        precedenceMap[MUL_ASSIGN] = ASSIGN_P;
        precedenceMap[DIV_ASSIGN] = ASSIGN_P;
        precedenceMap[MOD_ASSIGN] = ASSIGN_P;

        precedenceMap[EQ] = EQ_NEQ_P;
        precedenceMap[NEQ] = EQ_NEQ_P;
        precedenceMap[GT] = GT_LT_GTE_LTE_P;
        precedenceMap[LT] = GT_LT_GTE_LTE_P;
        precedenceMap[GTE] = GT_LT_GTE_LTE_P;
        precedenceMap[LTE] = GT_LT_GTE_LTE_P;

        precedenceMap[AND] = AND_P;
        precedenceMap[OR] = OR_P;
        precedenceMap[NOT] = SUB_INC_DEC_NOT_P;

        precedenceMap[BIT_AND] = BIT_AND_P;
        precedenceMap[BIT_OR] = BIT_OR_P;

        precedenceMap[LPAREN] = LPAREN_P;
    }

    //初始化词法标记的解析方法，前缀和中缀两个map。如果在map里没找到，会返回默认值null。
    void initParsingFunction() {
        f8PrefixParsingMap[IDENTIFIER] = std::bind(&Parser::parseIdentifierExpression, this);

        f8PrefixParsingMap[NULL_HKNI] = std::bind(&Parser::parseNullExpression, this);
        f8PrefixParsingMap[INT_VALUE] = std::bind(&Parser::parseIntExpression, this);
        f8PrefixParsingMap[FLOAT_VALUE] = std::bind(&Parser::parseFloatExpression, this);
        f8PrefixParsingMap[STRING_VALUE] = std::bind(&Parser::parseStringExpression, this);

        f8InfixParsingMap[ASSIGN] = std::bind(&Parser::parseAssignExpression, this, std::placeholders::_1);

        f8InfixParsingMap[ADD] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1);
        //负号和减号都是SUB，但是负号是前缀，减号是中缀
        f8PrefixParsingMap[SUB] = std::bind(&Parser::parsePrefixExpression, this);
        f8InfixParsingMap[SUB] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1);
        f8InfixParsingMap[MUL] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1);
        f8InfixParsingMap[DIV] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1);
        f8InfixParsingMap[MOD] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1);

        f8PrefixParsingMap[INC] = std::bind(&Parser::parsePrefixExpression, this);
        f8PrefixParsingMap[DEC] = std::bind(&Parser::parsePrefixExpression, this);

        f8InfixParsingMap[ADD_ASSIGN] = std::bind(&Parser::parseAssignExpression, this, std::placeholders::_1);
        f8InfixParsingMap[SUB_ASSIGN] = std::bind(&Parser::parseAssignExpression, this, std::placeholders::_1);
        f8InfixParsingMap[MUL_ASSIGN] = std::bind(&Parser::parseAssignExpression, this, std::placeholders::_1);
        f8InfixParsingMap[DIV_ASSIGN] = std::bind(&Parser::parseAssignExpression, this, std::placeholders::_1);
        f8InfixParsingMap[MOD_ASSIGN] = std::bind(&Parser::parseAssignExpression, this, std::placeholders::_1);

        f8InfixParsingMap[EQ] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1);
        f8InfixParsingMap[NEQ] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1);
        f8InfixParsingMap[GT] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1);
        f8InfixParsingMap[LT] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1);
        f8InfixParsingMap[GTE] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1);
        f8InfixParsingMap[LTE] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1);

        f8InfixParsingMap[AND] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1);
        f8InfixParsingMap[OR] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1);
        f8PrefixParsingMap[NOT] = std::bind(&Parser::parsePrefixExpression, this);

        f8InfixParsingMap[BIT_AND] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1);
        f8InfixParsingMap[BIT_OR] = std::bind(&Parser::parseInfixExpression, this, std::placeholders::_1);

        //左括号和函数调用都是LPAREN，但是左括号是前缀，函数调用是中缀
        f8PrefixParsingMap[LPAREN] = std::bind(&Parser::parseLParenExpression, this);
        f8InfixParsingMap[LPAREN] = std::bind(&Parser::parseCallExpression, this, std::placeholders::_1);

        f8PrefixParsingMap[TRUE_VALUE] = std::bind(&Parser::parseBoolExpression, this);
        f8PrefixParsingMap[FALSE_VALUE] = std::bind(&Parser::parseBoolExpression, this);

        f8PrefixParsingMap[IF_HKNI] = std::bind(&Parser::parseIfExpression, this);
        f8PrefixParsingMap[FOR_HKNI] = std::bind(&Parser::parseForExpression, this);

        f8PrefixParsingMap[FUNC_HKNI] = std::bind(&Parser::parseFuncExpression, this);
    }

    void getNextToken() {
        nowToken = nextToken;
        nextToken = p7lexer->GetNextToken();
    };

    bool nowTokenIs(TOKEN_TYPE type) {
        return nowToken.TokenType == type;
    };

    bool nextTokenIs(TOKEN_TYPE type) {
        return nextToken.TokenType == type;
    };

    void reportError() {
        string modeStr = this->p7lexer->GetModeStr();
        string rowStr = std::to_string(this->p7lexer->GetNowRow());
        string columnStr = std::to_string(this->p7lexer->GetNowColumn());

        string t4str = modeStr + rowStr + "行；" + columnStr + "列；expectNextTokenIs；语法错误：" + nowToken.Literal;
        errorList.push_back(t4str);
    }

    bool expectNextTokenIs(TOKEN_TYPE type) {
        if (nextTokenIs(type)) {
            getNextToken();
            return true;
        } else {
            reportError();
            return false;
        }
    }

    int getNowTokenPrecedence() {
        return precedenceMap[nowToken.TokenType];
    }

    int getNextTokenPrecedence() {
        return precedenceMap[nextToken.TokenType];
    }

    //####解析表达式

    //举例 1+2-3;
    //第一层parseExp，1拿到parseIntExp，优先级0<+，进while，+拿到parseInfixExp，进入
    //第一层parseInfixExp，left=1,op=+,right调用parseExp，传+优先级
    //第二层parseExp，2拿到parseIntExp，优先级+==-，不进while，返回2，退到第一层parseInfixExp
    //第一层parseInfixExp，right拿到2，返回left=1,op=+,right2，退到第一层parseExp
    //第一层parseExp，优先级0<-，进while，-拿到parseInfixExp，进入
    //第一层parseInfixExp，left={left=1,op=+,right=2},op=-,right调用parseExp，传-优先级
    //第二层parseExp，3拿到parseIntExp，遇到;，不进while，返回3，退到第一层parseInfixExp
    //第一层parseInfixExp，right拿到3，返回left={left=1,op=+,right=2},op=-,right=3，退到第一层parseExp
    //第一层parseExp，遇到;，不进while，返回left={left=1,op=+,right=2},op=-,right=3

    //举例 1+2*3;
    //第一层parseExp，1拿到parseIntExp，优先级0<+，进while，+拿到parseInfixExp，进入
    //第一层parseInfixExp，left=1,op=+,right调用parseExp，传+优先级
    //第二层parseExp，2拿到parseIntExp，优先级+<*，进while，*拿到parseInfixExp，进入
    //第二层parseInfixExp，left=2,op=*,right调用parseExp，传*优先级
    //第三层parseExp，3拿到parseIntExp，遇到;，不进while，返回3，退到第二层parseInfixExp
    //第二层parseInfixExp，right拿到3，返回left=2,op=*,right=3，退到第二层parseExp
    //第二层parseExp，遇到;，不进while，返回left=2,op=*,right=3，退到第一层parseInfixExp
    //第一层parseInfixExp，right拿到left=2,op=*,right=3，返回left=1,op=+,right={left=2,op=*,right=3}，退到第一层parseExp
    //第一层parseExp，遇到;，不进while，返回left=1,op=+,right={left=2,op=*,right=3}

    //举例 (1+2)*3;
    //第一层parseExp，(拿到parseLParenExp，进入
    //第一层parseLParenExp，调用parseExp，传0优先级
    //第二层parseExp，1拿到parseIntExp，优先级0<+，进while，+拿到parseInfixExp，进入
    //第二层parseInfixExp，left=1,op=+,right调用parseExp，传+优先级
    //第三层parseExp，2拿到parseIntExp，优先级+>0，不进while，返回2，退到第二层parseInfixExp
    //第二层parseInfixExp，right拿到2，返回left=1,op=+,right=2，退到第二层parseExp
    //第二层parseExp，优先级0==)，不进while，返回3，left=1,op=+,right=2，退到第一层parseLParenExp
    //第一层parseLParenExp，返回left=1,op=+,right=2，退到第一层parseExp
    //第一层parseExp，优先级0<*，进while，*拿到parseInfixExp，进入
    //第一层parseInfixExp，left={left=1,op=+,right=2},op=*,right调用parseExp，传*优先级
    //第二层parseExp，3拿到parseIntExp，遇到;，不进while，返回3，退到第一层parseInfixExp
    //第一层parseInfixExp，right拿到3，返回left={left=1,op=+,right=2},op=*,right=3，退到第一层parseExp
    //第一层parseExp，遇到;，不进while，返回left={left=1,op=+,right=2},op=*,right=3

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

                string t4str = modeStr + rowStr + "行；" + columnStr + "列；f8InfixParsing；语法错误：" + nowToken.Literal;
                errorList.push_back(t4str);

                return leftExp;
            }
            getNextToken();
            leftExp = f8InfixParsing(leftExp);
        }

        return leftExp;
    }

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
            reportError();
            return nullptr;
        }
        getNextToken();
        p7exp->I9ConditionStmt = parseStatement();
        if (!nowTokenIs(SEMICOLON)) {
            reportError();
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
        //func
        FuncExpression *p7exp = new FuncExpression(nowToken);

        //函数名
        if (nextTokenIs(IDENTIFIER)) {
            getNextToken();
            p7exp->P7NameExp = new IdentifierExpression(nowToken);
        }

        if (!expectNextTokenIs(LPAREN)) {
            return nullptr;
        }
        parseFuncArgsList(p7exp);

        if (!expectNextTokenIs(LBRACE)) {
            return nullptr;
        }
        p7exp->P7BodyBlockStmt = parseBlockStatement();
        return p7exp;
    }

    void parseFuncArgsList(FuncExpression *p7exp) {
        //无参数
        if (nextTokenIs(RPAREN)) {
            this->getNextToken();
            return;
        }

        //有参数
        std::vector<IdentifierExpression *> argList;
        this->getNextToken();
        argList.push_back(new IdentifierExpression(nowToken));
        while (nextTokenIs(COMMA)) {
            this->getNextToken();
            this->getNextToken();
            argList.push_back(new IdentifierExpression(nowToken));
        }

        expectNextTokenIs(RPAREN);

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
        argList.push_back(new IdentifierExpression(nowToken));
        while (nextTokenIs(COMMA)) {
            this->getNextToken();
            this->getNextToken();
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
        p7exp->I9RightExp = parseExpression(SUB_INC_DEC_NOT_P);
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

    //####解析表达式####

    //####解析语句

    //解析语句，递归入口
    I9Statement *parseStatement() {
        I9Statement *i9stmt;
        switch (nowToken.TokenType) {
            case BOOL_TYPE:
            case INT_TYPE:
            case FLOAT_TYPE:
            case STRING_TYPE:
                i9stmt = parseVarStatement();
                break;
            case RETURN_HKNI:
                i9stmt = parseReturnStatement();
                break;
            default:
                i9stmt = parseExpressionStatement();
                break;
        }
        return i9stmt;
    }

    VarStatement *parseVarStatement() {
        auto *p7stmt = new VarStatement(nowToken);

        if (!expectNextTokenIs(IDENTIFIER)) {
            return nullptr;
        }
        p7stmt->P7NameExp = new IdentifierExpression(nowToken);

        if (this->nextTokenIs(SEMICOLON)) {
            this->getNextToken();
            return p7stmt; //var 标识符表达式;
        }

        if (!expectNextTokenIs(ASSIGN)) {
            return nullptr;
        }
        this->getNextToken();
        p7stmt->I9ValueExp = parseExpression(LOWEST_P);
        if (this->nextTokenIs(SEMICOLON)) {
            this->getNextToken();
        }
        return p7stmt; //var 标识符表达式 = 表达式;
    }

    ReturnStatement *parseReturnStatement() {
        auto *p7stmt = new ReturnStatement(nowToken);
        this->getNextToken();
        p7stmt->I9Exp = parseExpression(LOWEST_P);
        if (this->nextTokenIs(SEMICOLON)) {
            this->getNextToken();
        }
        return p7stmt;
    }

    ExpressionStatement *parseExpressionStatement() {
        auto *p7stmt = new ExpressionStatement(nowToken);
        p7stmt->I9Exp = parseExpression(LOWEST_P);
        if (nextTokenIs(SEMICOLON)) {
            getNextToken();
        }
        return p7stmt;
    }

    BlockStatement *parseBlockStatement() {
        BlockStatement *p7stmt = new BlockStatement(nowToken);

        getNextToken(); //跳过'{'标记

        while (!nowTokenIs(RBRACE)) {
            if (nowTokenIs(COMMENT)) {
                getNextToken();
                continue;
            }

            auto *i9stmt = parseStatement();
            if (i9stmt != nullptr) {
                p7stmt->BodyStmtList.push_back(i9stmt);
            }

            getNextToken();
        }

        return p7stmt;
    }

    //####解析语句####
};

#endif //HKNI_PARSER_H
