#ifndef DEMO_CPP_PARSER_H
#define DEMO_CPP_PARSER_H

#include <map>
#include <functional>

#include "Lexer.h"

#include "ast/Precedence.h"

#include "ast/IdentifierExpression.h"
#include "ast/IntExpression.h"
#include "ast/FloatExpression.h"
#include "ast/StringExpression.h"
#include "ast/AssignExpression.h"

#include "ast/ExpressionStatement.h"
#include "ast/VarStatement.h"

#include "ast/Program.h"

using namespace ast;

//语法分析器
//基于运算符优先级和递归的实现
class Parser {
    //####属性
private:
    //类型别名
    using f8PrefixParsing = std::function<I9Expression *()>;
    using f8InfixParsing = std::function<I9Expression *(I9Expression *)>;

    Lexer *p7lexer;//词法分析器

    Token nowToken;
    Token nextToken;

    std::map<TOKEN_TYPE, PRECEDENCE> precedenceMap;//运算符优先级
    std::map<TOKEN_TYPE, f8PrefixParsing> f8PrefixParsingMap;//前缀运算符的解析方法
    std::map<TOKEN_TYPE, f8InfixParsing> f8InfixParsingMap;//中缀运算符的解析方法

    vector<string> errorList;//错误列表
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

    ~Parser() {};

    Program *DoParse() {
        Program *p7program = new Program();

        while (nowToken.TokenType != END) {
            if (nowToken.TokenType == ILLEGAL) {
                string modeStr = this->p7lexer->GetModeStr();
                string rowStr = std::to_string(this->p7lexer->GetNowRow());
                string columnStr = std::to_string(this->p7lexer->GetNowColumn());

                string t4str = modeStr + rowStr + "行；" + columnStr + "列；DoParse；语法错误：" + nowToken.Literal;
                errorList.push_back(t4str);
            } else if (nowToken.TokenType == COMMENT) {

            } else {
                I9Statement *i9stmt;
                switch (nowToken.TokenType) {
                    case HKNI_VAR:
                        i9stmt = parseVarStatement();
                        break;
                    default:
                        i9stmt = parseExpressionStatement();
                        break;
                }
                if (i9stmt != nullptr) {
                    p7program->I9StatementList.push_back(i9stmt);
                }
            }
            getNextToken();
        }

        return p7program;
    };
private:
    //初始化运算符优先级
    void initPrecedence() {
        precedenceMap[ASSIGN] = ASSIGN_P;
    }

    //初始化词法标记的解析方法
    void initParsingFunction() {
        f8PrefixParsingMap[IDENTIFIER] = std::bind(&Parser::parseIdentifierExpression, this);
        f8PrefixParsingMap[HKNI_INT] = std::bind(&Parser::parseIntExpression, this);
        f8PrefixParsingMap[HKNI_FLOAT] = std::bind(&Parser::parseFloatExpression, this);
        f8PrefixParsingMap[HKNI_STRING] = std::bind(&Parser::parseStringExpression, this);

        f8InfixParsingMap[ASSIGN] = std::bind(&Parser::parseAssignExpression, this, std::placeholders::_1);
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

    bool expectNextTokenIs(TOKEN_TYPE type) {
        if (nextTokenIs(type)) {
            getNextToken();
            return true;
        } else {
            string modeStr = this->p7lexer->GetModeStr();
            string rowStr = std::to_string(this->p7lexer->GetNowRow());
            string columnStr = std::to_string(this->p7lexer->GetNowColumn());

            string t4str = modeStr + rowStr + "行；" + columnStr + "列；expectNextTokenIs；语法错误：" + nowToken.Literal;
            errorList.push_back(t4str);

            return false;
        }
    }

    int getNextTokenPrecedence() {
        return precedenceMap[nextToken.TokenType];
    }

    //####解析表达式

    I9Expression *parseExpression(PRECEDENCE precedence) {
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

        //递归，这里用 1+2*3 举例
        while (!nextTokenIs(SEMICOLON) && precedence < getNextTokenPrecedence()) {
            // 进来这里肯定是中缀运算符
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

    IntExpression *parseIntExpression() {
        return new IntExpression(nowToken);
    };

    FloatExpression *parseFloatExpression() {
        return new FloatExpression(nowToken);
    };

    StringExpression *parseStringExpression() {
        return new StringExpression(nowToken);
    };

    AssignExpression *parseAssignExpression(I9Expression *i9exp) {
        AssignExpression *p7exp = new AssignExpression(nowToken);
        p7exp->i9NameExp = i9exp;
        getNextToken();
        p7exp->i9ValueExp = parseExpression(LOWEST_P);
        return p7exp;
    }

    //####解析表达式####

    //####解析语句

    //表达式;
    ExpressionStatement *parseExpressionStatement() {
        ExpressionStatement *p7stmt = new ExpressionStatement(nowToken);
        p7stmt->I9Exp = parseExpression(LOWEST_P);
        if (nextTokenIs(SEMICOLON)) {
            getNextToken();
        }
        return p7stmt;
    }

    //var 标识符 = 表达式;
    VarStatement *parseVarStatement() {
        auto *p7stmt = new VarStatement(nowToken);

        if (!expectNextTokenIs(IDENTIFIER)) {
            return nullptr;
        }
        p7stmt->p7NameExp = new IdentifierExpression(nowToken);

        if (!expectNextTokenIs(ASSIGN)) {
            return nullptr;
        }
        this->getNextToken();

        p7stmt->i9ValueExp = parseExpression(LOWEST_P);

        if (this->nextTokenIs(SEMICOLON)) {
            this->getNextToken();
        }

        return p7stmt;
    }

    //####解析语句####
};

#endif //DEMO_CPP_PARSER_H
