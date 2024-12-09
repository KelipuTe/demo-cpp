#include <iostream>

#include "Parser.h"

using namespace hkni;

//语法分析测试用例
class ParserTestCase {
public:
    string name;
    string input;
    LexerMode mode;
    ParserTestCase(string name, string input, LexerMode mode) {
        this->name = name;
        this->input = input;
        this->mode = mode;
    }
};

void testParserDoParse() {
    //写代码的顺序和TokenType那里的顺序一样，方便找。
    ParserTestCase *tcList[] = {
            // new ParserTestCase("STRING_VALUE", "\"a\";", INPUT_MODE), //a
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

            // new ParserTestCase("FLOAT_TYPE", "var a float;", INPUT_MODE),
            //new ParserTestCase("FLOAT_TYPE", "var a float = 3.14;", INPUT_MODE),

            //new ParserTestCase("STRING_TYPE", "var a string;", INPUT_MODE),
            //new ParserTestCase("STRING_TYPE", "var a string = \"a\";", INPUT_MODE),

            //new ParserTestCase("IDENTIFIER", "a;", INPUT_MODE),

            //new ParserTestCase("IF", "if(a==2){b=10;}", INPUT_MODE),
            //new ParserTestCase("IF", "if(a==2){b=10;}else{b=20;}", INPUT_MODE),
            //new ParserTestCase("IF", "if(a==2 && c==2){b=10;}", INPUT_MODE),

            //new ParserTestCase("FOR", "for(i=1;i<10;++i){j+=2;}", INPUT_MODE),

            //new ParserTestCase("FUNC", "func hello(){}", INPUT_MODE),
            //new ParserTestCase("FUNC", "func hello(a int){}", INPUT_MODE),
            //new ParserTestCase("FUNC", "func hello(a int,b int){}", INPUT_MODE),
            //new ParserTestCase("FUNC", "func hello() int {return 1;}", INPUT_MODE),
            //new ParserTestCase("FUNC", "func hello() int {var a int=1;return a;}", INPUT_MODE),

            //new ParserTestCase("CALL", "hello();", INPUT_MODE),
            //new ParserTestCase("CALL", "hello(1);", INPUT_MODE),
            //new ParserTestCase("CALL", "hello(1,2);", INPUT_MODE),
            //new ParserTestCase("CALL", "hello(a);", INPUT_MODE),
            //new ParserTestCase("CALL", "hello(a,b);", INPUT_MODE),
            //new ParserTestCase("CALL", "hello(a());", INPUT_MODE),
            //new ParserTestCase("CALL", "hello(a(),b());", INPUT_MODE),

            //new ParserTestCase("RETURN", "return;", INPUT_MODE),
            //new ParserTestCase("RETURN", "return 2;", INPUT_MODE),
            //new ParserTestCase("RETURN", "return 2+2;", INPUT_MODE),
            //new ParserTestCase("RETURN", "return a;", INPUT_MODE),
            //new ParserTestCase("RETURN", "return a+b;", INPUT_MODE),
            //new ParserTestCase("RETURN", "return a();", INPUT_MODE),
            //new ParserTestCase("RETURN", "return a()+b();", INPUT_MODE),

        // new ParserTestCase("MAP", "map[string]string;", INPUT_MODE),
        new ParserTestCase("MAP", "var a map[string]string;", INPUT_MODE),
        // new ParserTestCase("MAP", "map[string]string{};", INPUT_MODE),
        // new ParserTestCase("MAP", "map[string]string{\"k1\":\"v1\"};", INPUT_MODE),
        // new ParserTestCase("MAP", "map[string]string{\"k1\":\"v1\",\"k2\":\"v2\"};", INPUT_MODE),

    };

    int tcListLen = sizeof(tcList) / sizeof(tcList[0]);
    for (int i = 0; i < tcListLen; i++) {
        cout << "test case[" << i << "] " << tcList[i]->name << ":" << endl;

        auto *lexer = new Lexer(tcList[i]->input, tcList[i]->mode);
        auto *parser = new Parser(lexer);
        auto *program = parser->DoParse();

        if (parser->HasError()) {
            parser->PrintError();
        } else {
            for (auto item: program->StmtI9List) {
                cout << item->ToString() << endl;
            }
        }

        delete program;
        delete parser;
        delete lexer;
    }
}

int main() {
    testParserDoParse();
}
