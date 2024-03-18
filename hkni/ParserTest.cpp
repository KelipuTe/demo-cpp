#include <iostream>

#include "Parser.h"

using namespace std;

using namespace hkni;

class ParserTestCase {
public:
    string name;
    string input;
    LexerMode mode;
public:
    ParserTestCase(string name, string input, LexerMode mode) {
        this->name = name;
        this->input = input;
        this->mode = mode;
    }
};

void testParserDoParse() {
    ParserTestCase *tcList[] = {
            //运算符
            //new ParserTestCase("STRING_VALUE", "\"a\";", INPUT_MODE), //a
            //new ParserTestCase("STRING_VALUE", "\"ab\";", INPUT_MODE), //ab
            //new ParserTestCase("STRING_VALUE", "\"a'b'\";", INPUT_MODE), //a'b'
            //new ParserTestCase("STRING_VALUE", "\"a\\\"b\\\"\";", INPUT_MODE), //a"b"
            //new ParserTestCase("STRING_VALUE", "\"a\\b\";", INPUT_MODE), //a\b
            //new ParserTestCase("STRING_VALUE", "\"a\\nb\";", INPUT_MODE), //a\nb
            //new ParserTestCase("NOT", "!true;", INPUT_MODE),
            //new ParserTestCase("NEQ", "2!=2;", INPUT_MODE),
            //new ParserTestCase("NEQ", "a!=2;", INPUT_MODE),
            //new ParserTestCase("MOD", "10%2;", INPUT_MODE),
            //new ParserTestCase("MOD_ASSIGN", "a%=2;", INPUT_MODE),
            //new ParserTestCase("BIT_AND", "0b10 & 0b11;", INPUT_MODE),
            //new ParserTestCase("AND", "true && false;", INPUT_MODE),
            //new ParserTestCase("MUL", "10*2;", INPUT_MODE),
            //new ParserTestCase("MUL_ASSIGN", "a*=2;", INPUT_MODE),
            //new ParserTestCase("LPAREN", "(2+2)*2;", INPUT_MODE),
            //new ParserTestCase("LPAREN", "-(2+2);", INPUT_MODE),
            //new ParserTestCase("SUB", "10-2;", INPUT_MODE),
            //new ParserTestCase("SUB", "-2;", INPUT_MODE),
            //new ParserTestCase("SUB", "-2+2;", INPUT_MODE),
            //new ParserTestCase("SUB_ASSIGN", "a-=2;", INPUT_MODE),
            //new ParserTestCase("DEC", "--i;", INPUT_MODE),
            //new ParserTestCase("ADD", "10+2;", INPUT_MODE),
            //new ParserTestCase("ADD_ASSIGN", "a+=2;", INPUT_MODE),
            //new ParserTestCase("INC", "++i;", INPUT_MODE),
            //new ParserTestCase("ASSIGN", "a=2;", INPUT_MODE),
            //new ParserTestCase("ASSIGN", "a=b;", INPUT_MODE),
            //new ParserTestCase("EQ", "2==2;", INPUT_MODE),
            //new ParserTestCase("EQ", "a==2;", INPUT_MODE),
            //new ParserTestCase("BIT_OR", "0b10 | 0b11;", INPUT_MODE),
            //new ParserTestCase("OR", "true || false;", INPUT_MODE),
            //new ParserTestCase("LT", "2<2;", INPUT_MODE),
            //new ParserTestCase("LT", "a<2;", INPUT_MODE),
            //new ParserTestCase("LTE", "2<=2;", INPUT_MODE),
            //new ParserTestCase("LTE", "a<=2;", INPUT_MODE),
            //new ParserTestCase("GT", "2>2;", INPUT_MODE),
            //new ParserTestCase("GT", "a>2;", INPUT_MODE),
            //new ParserTestCase("GTE", "1>=2;", INPUT_MODE),
            //new ParserTestCase("GTE", "a>=2;", INPUT_MODE),
            //new ParserTestCase("DIV", "10/2;", INPUT_MODE),
            //new ParserTestCase("DIV_ASSIGN", "a/=2;", INPUT_MODE),
            //new ParserTestCase("COMMENT", "//单行注释", INPUT_MODE),

            //new ParserTestCase("ADD_SUB", "10+2-3;", INPUT_MODE),
            //new ParserTestCase("ADD_MUL", "10+2*3;", INPUT_MODE),
            //new ParserTestCase("MUL_DIV", "10*2/3;", INPUT_MODE),

            //关键字
            //new ParserTestCase("NULL", "null;", INPUT_MODE),
            //new ParserTestCase("BOOL_VALUE", "true;", INPUT_MODE),
            //new ParserTestCase("BOOL_VALUE", "false;", INPUT_MODE),
            //new ParserTestCase("BOOL_TYPE", "var a bool;", INPUT_MODE),
            //new ParserTestCase("BOOL_TYPE", "var a bool = true;", INPUT_MODE),
            //new ParserTestCase("INT_VALUE", "0b10;", INPUT_MODE),
            //new ParserTestCase("INT_VALUE", "010;", INPUT_MODE),
            //new ParserTestCase("INT_VALUE", "10;", INPUT_MODE),
            //new ParserTestCase("INT_VALUE", "0x10;", INPUT_MODE),
            //new ParserTestCase("INT_TYPE", "var a int;", INPUT_MODE),
            //new ParserTestCase("INT_TYPE", "var a int = 2;", INPUT_MODE),
            //new ParserTestCase("FLOAT_VALUE", "3.00;", INPUT_MODE),
            //new ParserTestCase("FLOAT_VALUE", "3.14;", INPUT_MODE),
            //new ParserTestCase("FLOAT_TYPE", "var a float;", INPUT_MODE),
            //new ParserTestCase("FLOAT_TYPE", "var a float = 3.14;", INPUT_MODE),
            //new ParserTestCase("STRING_TYPE", "var a string;", INPUT_MODE),
            //new ParserTestCase("STRING_TYPE", "var a string = \"a\";", INPUT_MODE),
            //new ParserTestCase("IDENTIFIER", "a;", INPUT_MODE),
            //new ParserTestCase("IF_HKNI", "if(a==2){b=10;}", INPUT_MODE),
            //new ParserTestCase("ELSE_HKNI", "if(a==2){b=10;}else{b=20;}", INPUT_MODE),
            //new ParserTestCase("IF", "if(a==2 && c==2){b=10;}", INPUT_MODE),
            //new ParserTestCase("FOR_HKNI", "for(i=0;i<10;++i){j+=2;}", INPUT_MODE),
            //new ParserTestCase("FUNCTION_HKNI", "function hello(){}", INPUT_MODE),
            //new ParserTestCase("FUNCTION_HKNI", "function hello(a int){}", INPUT_MODE),
            //new ParserTestCase("FUNCTION_HKNI", "function hello(a int,b int){}", INPUT_MODE),
            //new ParserTestCase("FUNCTION_HKNI", "function hello() int {return 1;}", INPUT_MODE),
            //new ParserTestCase("FUNCTION_HKNI", "function hello() int {var a int=1;return a;}", INPUT_MODE),
            //new ParserTestCase("FUNCTION_HKNI", "hello();", INPUT_MODE),
            //new ParserTestCase("FUNCTION_HKNI", "hello(1);", INPUT_MODE),
            //new ParserTestCase("FUNCTION_HKNI", "hello(1,2);", INPUT_MODE),
            //new ParserTestCase("FUNCTION_HKNI", "hello(a);", INPUT_MODE),
            //new ParserTestCase("FUNCTION_HKNI", "hello(a,b);", INPUT_MODE),
            //new ParserTestCase("FUNCTION_HKNI", "hello(a());", INPUT_MODE),
            //new ParserTestCase("FUNCTION_HKNI", "hello(a(),b());", INPUT_MODE),
            //new ParserTestCase("RETURN_HKNI", "return;", INPUT_MODE),
            //new ParserTestCase("RETURN_HKNI", "return 2;", INPUT_MODE),
            //new ParserTestCase("RETURN_HKNI", "return 2+2;", INPUT_MODE),
            //new ParserTestCase("RETURN_HKNI", "return a;", INPUT_MODE),
            //new ParserTestCase("RETURN_HKNI", "return a+b;", INPUT_MODE),
            //new ParserTestCase("RETURN_HKNI", "return a();", INPUT_MODE),
            //new ParserTestCase("RETURN_HKNI", "return a()+b();", INPUT_MODE),
    };

    int tcListLen = sizeof(tcList) / sizeof(tcList[0]);
    for (int i = 0; i < tcListLen; i++) {
        cout << "test case[" << i << "] " << tcList[i]->name << ":" << endl;

        Lexer *p7lexer = new Lexer(tcList[i]->input, tcList[i]->mode);
        Parser *p7parser = new Parser(p7lexer);
        Program *p7program = p7parser->DoParse();

        if (p7parser->HasError()) {
            p7parser->PrintError();
        } else {
            for (auto item: p7program->I9StatementList) {
                cout << item->ToString() << endl;
            }
        }

        delete p7program;
        delete p7parser;
        delete p7lexer;
    }
}

int main() {
    testParserDoParse();
}
