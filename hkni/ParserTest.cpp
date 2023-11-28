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
//            new ParserTestCase("IDENTIFIER", "a;", INPUT_MODE),
//
//            new ParserTestCase("HKNI_INT", "10;", INPUT_MODE),
//            new ParserTestCase("HKNI_INT", "0b10;", INPUT_MODE),
//            new ParserTestCase("HKNI_INT", "010;", INPUT_MODE),
//            new ParserTestCase("HKNI_INT", "0x10;", INPUT_MODE),
//            new ParserTestCase("HKNI_FLOAT", "10.10;", INPUT_MODE),
//            new ParserTestCase("HKNI_STRING", "\"aaa\";", INPUT_MODE),
//
//            new ParserTestCase("ASSIGN", "a=1;", INPUT_MODE),

//            new ParserTestCase("ADD", "1+2", INPUT_MODE),
//            new ParserTestCase("SUB", "1-2", INPUT_MODE),
//            new ParserTestCase("ADD_SUB", "1+2-3", INPUT_MODE),
//            new ParserTestCase("MUL", "1*2", INPUT_MODE),
//            new ParserTestCase("DIV", "1/2", INPUT_MODE),
//            new ParserTestCase("MUL_DIV", "1*2/3", INPUT_MODE),
//            new ParserTestCase("MOD", "1%2", INPUT_MODE),
//            new ParserTestCase("ADD_MUL", "1+2*3", INPUT_MODE),
            new ParserTestCase("ADD_LPAREN_MUL", "(1+2)*3", INPUT_MODE),

//            new ParserTestCase("INC", "i++", INPUT_MODE),
//            new ParserTestCase("DEC", "i--", INPUT_MODE),

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
