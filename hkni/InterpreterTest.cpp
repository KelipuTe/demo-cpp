#include <iostream>
#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"

using namespace std;

class InterpreterTestCase {
public:
    string name;
    string input;
    int mode;
public:
    InterpreterTestCase(string name, string input, int mode) {
        this->name = name;
        this->input = input;
        this->mode = mode;
    }

    ~InterpreterTestCase() = default;
};

void testInterpreterDoInterpreter() {
    InterpreterTestCase *tcList[] = {
//            new InterpreterTestCase("INT_HKNI", "10;", INPUT_MODE),
//            new InterpreterTestCase("INT_HKNI", "0b10;", INPUT_MODE),
//            new InterpreterTestCase("INT_HKNI", "010;", INPUT_MODE),
//            new InterpreterTestCase("INT_HKNI", "0x10;", INPUT_MODE),
            //            new InterpreterTestCase("FLOAT_HKNI", "10.10;", INPUT_MODE),
            //            new InterpreterTestCase("STRING_HKNI", "\"aaa\";", INPUT_MODE),

            new InterpreterTestCase("VAR", "var a=1;", INPUT_MODE),
    };

    int tcListLen = sizeof(tcList) / sizeof(tcList[0]);
    for (int i = 0; i < tcListLen; i++) {
        Lexer *p7lexer = new Lexer(tcList[i]->input, tcList[i]->mode);
        Parser *p7parser = new Parser(p7lexer);
        Program *p7program = p7parser->DoParse();

        Interpreter *p7interpreter = new Interpreter();
        Environment *p7env = new Environment();
        Object *p7obj = p7interpreter->DoInterpret(p7program, p7env);
        cout << p7obj->GetValue() << endl;

        delete p7program;
        delete p7parser;
        delete p7lexer;
        delete p7interpreter;

        cout << "test case:" << tcList[i]->name << " done.\n" << endl;
    }
}

int main() {
    testInterpreterDoInterpreter();
}
