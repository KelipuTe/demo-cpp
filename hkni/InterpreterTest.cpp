#include <iostream>

#include "Parser.h"
#include "Interpreter.h"

using namespace std;

class InterpreterTestCase {
public:
    string name;
    string input;
    LexerMode mode;
public:
    InterpreterTestCase(string name, string input, LexerMode mode) {
        this->name = name;
        this->input = input;
        this->mode = mode;
    }
};

void testInterpreterDoInterpreter() {
    InterpreterTestCase *tcList[] = {
//            new InterpreterTestCase("BOOL", "bool a;", INPUT_MODE),
//            new InterpreterTestCase("BOOL", "true;", INPUT_MODE),
//            new InterpreterTestCase("BOOL", "false;", INPUT_MODE),
//
//            new InterpreterTestCase("INT", "int a;", INPUT_MODE),
//            new InterpreterTestCase("INT", "10;", INPUT_MODE),
//            new InterpreterTestCase("INT", "0b10;", INPUT_MODE),
//            new InterpreterTestCase("INT", "010;", INPUT_MODE),
//            new InterpreterTestCase("INT", "0x10;", INPUT_MODE),
//
//            new InterpreterTestCase("FLOAT", "float a;", INPUT_MODE),
//            new InterpreterTestCase("FLOAT", "10.10;", INPUT_MODE),
//            new InterpreterTestCase("FLOAT", "10.50;", INPUT_MODE),
//
//            new InterpreterTestCase("STRING", "string a;", INPUT_MODE),
//            new InterpreterTestCase("STRING", "\"a\";", INPUT_MODE),
//            new InterpreterTestCase("STRING", "\"aa\";", INPUT_MODE),
//            new InterpreterTestCase("STRING", "\"aa\\nbb\";", INPUT_MODE),
//            new InterpreterTestCase("STRING", "\"aa'bb'\";", INPUT_MODE),
//            new InterpreterTestCase("STRING", "\"aa\\\"bb\\\"\";", INPUT_MODE),
//
//            new InterpreterTestCase("ADD", "1+2;", INPUT_MODE),
//            new InterpreterTestCase("ADD", "1.1+2.2;", INPUT_MODE),
//            new InterpreterTestCase("SUB", "1-2;", INPUT_MODE),
//            new InterpreterTestCase("SUB", "1.1-2.1;", INPUT_MODE),
//            new InterpreterTestCase("MUL", "1*2;", INPUT_MODE),
//            new InterpreterTestCase("MUL", "1.1*2.2;", INPUT_MODE),
//            new InterpreterTestCase("DIV", "1/2;", INPUT_MODE),
//            new InterpreterTestCase("DIV", "1.1/2.2;", INPUT_MODE),
//            new InterpreterTestCase("MOD", "1%2;", INPUT_MODE),
//
//            new InterpreterTestCase("ADD_SUB", "1+2-3;", INPUT_MODE),
//            new InterpreterTestCase("ADD_MUL", "1+2*3;", INPUT_MODE),
//            new InterpreterTestCase("MUL_DIV", "1*2/3;", INPUT_MODE),
//
//            new InterpreterTestCase("SUB", "-2;", INPUT_MODE),
//            new InterpreterTestCase("SUB", "-2-2;", INPUT_MODE),
//
//            new InterpreterTestCase("LPAREN", "1+(2*3);", INPUT_MODE),
//            new InterpreterTestCase("LPAREN", "(1+2)*3;", INPUT_MODE),
//            new InterpreterTestCase("LPAREN", "-(2+2);", INPUT_MODE),
//
//            new InterpreterTestCase("EQ", "1==1;", INPUT_MODE),
//            new InterpreterTestCase("EQ", "int a=1;a==1;", INPUT_MODE),
//            new InterpreterTestCase("NEQ", "1!=1;", INPUT_MODE),
//            new InterpreterTestCase("NEQ", "int a=1;a!=1;", INPUT_MODE),
//            new InterpreterTestCase("GT", "1>1;", INPUT_MODE),
//            new InterpreterTestCase("GT", "int a=1;a>1;", INPUT_MODE),
//            new InterpreterTestCase("GTE", "1>=1;", INPUT_MODE),
//            new InterpreterTestCase("GTE", "int a=1;a>=1;", INPUT_MODE),
//            new InterpreterTestCase("LT", "1<1;", INPUT_MODE),
//            new InterpreterTestCase("LT", "int a=1;a<1;", INPUT_MODE),
//            new InterpreterTestCase("LTE", "1<=1;", INPUT_MODE),
//            new InterpreterTestCase("LTE", "int a=1;a<=1;", INPUT_MODE),
//
//            new InterpreterTestCase("AND", "true && false;", INPUT_MODE),
//            new InterpreterTestCase("AND", "bool a = true; bool b = false;a && b;", INPUT_MODE),
//            new InterpreterTestCase("OR", "true || false;", INPUT_MODE),
//            new InterpreterTestCase("OR", "bool a = true; bool b = false;a || b;", INPUT_MODE),
//            new InterpreterTestCase("NOT", "!true;", INPUT_MODE),
//            new InterpreterTestCase("NOT", "bool a = true;!a;", INPUT_MODE),

//            new InterpreterTestCase("BIT_AND", "0b10 & 0b11;", INPUT_MODE),
//            new InterpreterTestCase("BIT_OR", "0b10 | 0b11;", INPUT_MODE),

//            new InterpreterTestCase("ASSIGN", "int a;a=1;a;", INPUT_MODE),
//            new InterpreterTestCase("ASSIGN", "int a=1;int b=2;a=b;a;", INPUT_MODE),

//            new InterpreterTestCase("ADD_ASSIGN", "int a=1;a+=2;a;", INPUT_MODE),
//            new InterpreterTestCase("SUB_ASSIGN", "int a=1;a-=2;a;", INPUT_MODE),
//            new InterpreterTestCase("MUL_ASSIGN", "int a=1;a*=2;a;", INPUT_MODE),
//            new InterpreterTestCase("DIV_ASSIGN", "int a=1;a/=2;a;", INPUT_MODE),
//            new InterpreterTestCase("MOD_ASSIGN", "int a=1;a%=2;a;", INPUT_MODE),
//
//            new InterpreterTestCase("INC", "int i=1;++i;", INPUT_MODE),
//            new InterpreterTestCase("DEC", "int i=1;--i;", INPUT_MODE),

//            new InterpreterTestCase("IF", "int a = 1;int b;if(a==1){b=1;}b;", INPUT_MODE),
//            new InterpreterTestCase("IF", "int a = 1;int b;if(a==1){b=1;}else{b=2;}b;", INPUT_MODE),
//            new InterpreterTestCase("IF", "int a = 1;int b;if(a==2){b=1;}else{b=2;}b;", INPUT_MODE),
//            new InterpreterTestCase("IF", "int a = 1;int b;int c = 1;if(a==1 && c==1){b=1;}b;", INPUT_MODE),

//            new InterpreterTestCase("CALL", "function hello(){} hello();", INPUT_MODE),

            new InterpreterTestCase("BUILTIN_FUNC", "println(\"aaa\");", INPUT_MODE),
    };

    int tcListLen = sizeof(tcList) / sizeof(tcList[0]);
    for (int i = 0; i < tcListLen; i++) {
        Lexer *p7lexer = new Lexer(tcList[i]->input, tcList[i]->mode);
        Parser *p7parser = new Parser(p7lexer);
        Program *p7program = p7parser->DoParse();

        Interpreter *p7interpreter = new Interpreter();

        BuiltinFunc* builtinFunctions = new BuiltinFunc();
        p7interpreter->registerBuiltinFunctions(builtinFunctions);

        Environment *p7env = new Environment();
        Object *p7obj = p7interpreter->DoInterpret(p7program, p7env);
        if (p7obj != nullptr) {
            cout << p7obj->ToString() << endl;
        }

        delete p7program;
        delete p7parser;
        delete p7lexer;
        delete p7interpreter;

        cout << "test case:[" << i << "]:" << tcList[i]->name << " done.\n" << endl;
    }
}

int main() {
    testInterpreterDoInterpreter();
}
