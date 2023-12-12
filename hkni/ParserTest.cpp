#include <iostream>
#include "Token.h"
#include "Lexer.h"
#include "Parser.h"
#include "ast/Program.h"

using namespace std;

class ParserTestCase {
public:
    string name;
    string input;
    int mode;
public:
    ParserTestCase(string name, string input, int mode) {
        this->name = name;
        this->input = input;
        this->mode = mode;
    }

    ~ParserTestCase() = default;
};

void testParserDoParse() {
    ParserTestCase *tcList[] = {
//                        new ParserTestCase("COMMENT", "//a", INPUT_MODE),

//            new ParserTestCase("IDENTIFIER", "a;", INPUT_MODE),

//            new ParserTestCase("NULL_HKNI", "null;", INPUT_MODE),
//            new ParserTestCase("INT_HKNI", "10;", INPUT_MODE),
//            new ParserTestCase("INT_HKNI", "0b10;", INPUT_MODE),
//            new ParserTestCase("INT_HKNI", "010;", INPUT_MODE),
//            new ParserTestCase("INT_HKNI", "0x10;", INPUT_MODE),
//            new ParserTestCase("FLOAT_HKNI", "10.00;", INPUT_MODE),
//            new ParserTestCase("FLOAT_HKNI", "10.50;", INPUT_MODE),
//            new ParserTestCase("STRING_HKNI", "\"a\";", INPUT_MODE),
//            new ParserTestCase("STRING_HKNI", "\"aa\";", INPUT_MODE),
//            new ParserTestCase("STRING_HKNI", "\"aa\\nbb\";", INPUT_MODE),
//            new ParserTestCase("STRING_HKNI", "\"aa'bb'\";", INPUT_MODE),
//            new ParserTestCase("STRING_HKNI", "\"aa\\\"bb\\\"\";", INPUT_MODE),

//            new ParserTestCase("ASSIGN", "a=1;", INPUT_MODE),

//            new ParserTestCase("ADD", "1+2;", INPUT_MODE),
//            new ParserTestCase("SUB", "1-2;", INPUT_MODE),
//            new ParserTestCase("SUB", "-2;", INPUT_MODE),
//            new ParserTestCase("SUB_SUB", "-2-2;", INPUT_MODE),
//            new ParserTestCase("ADD_SUB", "1+2-3;", INPUT_MODE),
//            new ParserTestCase("MUL", "1*2;", INPUT_MODE),
//            new ParserTestCase("DIV", "1/2;", INPUT_MODE),
//            new ParserTestCase("MUL_DIV", "1*2/3;", INPUT_MODE),
//            new ParserTestCase("MOD", "1%2;", INPUT_MODE),
//            new ParserTestCase("ADD_MUL", "1+2*3;", INPUT_MODE),
//            new ParserTestCase("ADD_LPAREN_MUL", "(1+2)*3;", INPUT_MODE),

//            new ParserTestCase("INC", "++i;", INPUT_MODE),
//            new ParserTestCase("DEC", "--i;", INPUT_MODE),

//            new ParserTestCase("ADD_ASSIGN", "a+=2;", INPUT_MODE),
//            new ParserTestCase("SUB_ASSIGN", "a-=2;", INPUT_MODE),
//            new ParserTestCase("MUL_ASSIGN", "a*=2;", INPUT_MODE),
//            new ParserTestCase("DIV_ASSIGN", "a/=2;", INPUT_MODE),
//            new ParserTestCase("MOD_ASSIGN", "a%=2;", INPUT_MODE),

//            new ParserTestCase("EQ", "a==1;", INPUT_MODE),
//            new ParserTestCase("NEQ", "a!=1;", INPUT_MODE),
//            new ParserTestCase("GT", "a>1;", INPUT_MODE),
//            new ParserTestCase("GTE", "a>=1;", INPUT_MODE),
//            new ParserTestCase("LT", "a<1;", INPUT_MODE),
//            new ParserTestCase("LTE", "a<=1;", INPUT_MODE),

//            new ParserTestCase("AND", "true && false;", INPUT_MODE),
//            new ParserTestCase("OR", "true || false;", INPUT_MODE),
//            new ParserTestCase("NOT", "!true;", INPUT_MODE),

//            new ParserTestCase("BIT_AND", "0b10 & 0b11;", INPUT_MODE),
//            new ParserTestCase("BIT_OR", "0b10 | 0b11;", INPUT_MODE),

//            new ParserTestCase("TRUE_HKNI", "true;", INPUT_MODE),
//            new ParserTestCase("FALSE_HKNI", "false;", INPUT_MODE),

//            new ParserTestCase("VAR", "var a=1;",  INPUT_MODE),

//            new ParserTestCase("IF", "if(a==1){b=1;}", INPUT_MODE),
//            new ParserTestCase("IF", "if(a==1){b=1;}else{b=2;}", INPUT_MODE),

//            new ParserTestCase("FOR", "for(i=1;i<10;i+=1){}", INPUT_MODE),

//            new ParserTestCase("FUNC", "func hello(){}", INPUT_MODE),
//            new ParserTestCase("FUNC", "func hello(a){a;}", INPUT_MODE),
//            new ParserTestCase("FUNC", "func hello(a,b){a;b;}", INPUT_MODE),

//new ParserTestCase("RETURN", "return;", INPUT_MODE),
//new ParserTestCase("RETURN", "return 1;", INPUT_MODE),
//new ParserTestCase("RETURN", "return a;", INPUT_MODE),

//            new ParserTestCase("CALL", "hello();", INPUT_MODE),
//            new ParserTestCase("CALL", "hello(1);", INPUT_MODE),
//            new ParserTestCase("CALL", "hello(1,2);", INPUT_MODE),
//            new ParserTestCase("CALL", "hello(a);", INPUT_MODE),
//            new ParserTestCase("CALL", "hello(a,b);", INPUT_MODE),
//            new ParserTestCase("CALL", "hello(pie());", INPUT_MODE),
    };

    int tcListLen = sizeof(tcList) / sizeof(tcList[0]);
    for (int i = 0; i < tcListLen; i++) {
        Lexer *p7lexer = new Lexer(tcList[i]->input, tcList[i]->mode);
        Parser *p7parser = new Parser(p7lexer);
        Program *p7program = p7parser->DoParse();

        for (auto item: p7program->I9StatementList) {
            cout << item->ToString() << endl;
        }

        delete p7program;
        delete p7parser;
        delete p7lexer;

        cout << "test case:" << tcList[i]->name << " done.\n" << endl;
    }
}

int main() {
    testParserDoParse();
}
