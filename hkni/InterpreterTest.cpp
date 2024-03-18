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
            //运算符
            //new InterpreterTestCase("STRING_VALUE", "\"a\";", INPUT_MODE), //a
            //new InterpreterTestCase("STRING_VALUE", "\"ab\";", INPUT_MODE), //ab
            //new InterpreterTestCase("STRING_VALUE", "\"a'b'\";", INPUT_MODE), //a'b'
            //new InterpreterTestCase("STRING_VALUE", "\"a\\\"b\\\"\";", INPUT_MODE), //a"b"
            //new InterpreterTestCase("STRING_VALUE", "\"a\\b\";", INPUT_MODE), //a\b
            //new InterpreterTestCase("STRING_VALUE", "\"a\\nb\";", INPUT_MODE), //a\nb
            //new InterpreterTestCase("NOT", "!true;", INPUT_MODE),
            //new InterpreterTestCase("NOT", "var a bool=true;!a;", INPUT_MODE),
            //new InterpreterTestCase("NEQ", "1!=1;", INPUT_MODE),
            //new InterpreterTestCase("NEQ", "var a int=1;a!=1;", INPUT_MODE),
            //new InterpreterTestCase("MOD", "1%2;", INPUT_MODE),
            //new InterpreterTestCase("MOD_ASSIGN", "var a int=1;a%=2;a;", INPUT_MODE),
            //new InterpreterTestCase("BIT_AND", "0b10 & 0b11;", INPUT_MODE),
            //new InterpreterTestCase("AND", "true && false;", INPUT_MODE),
            //new InterpreterTestCase("AND", "var a bool=true; var b bool=false;a && b;", INPUT_MODE),
            //new InterpreterTestCase("MUL", "1*2;", INPUT_MODE),
            //new InterpreterTestCase("MUL", "1.1*2.2;", INPUT_MODE),
            //new InterpreterTestCase("MUL_ASSIGN", "var a int=1;a*=2;a;", INPUT_MODE),
            //new InterpreterTestCase("LPAREN", "1+(2*3);", INPUT_MODE),
            //new InterpreterTestCase("LPAREN", "(1+2)*3;", INPUT_MODE),
            //new InterpreterTestCase("LPAREN", "-(2+2);", INPUT_MODE),
            //new InterpreterTestCase("SUB", "-2;", INPUT_MODE),
            //new InterpreterTestCase("SUB", "-2-2;", INPUT_MODE),
            //new InterpreterTestCase("SUB", "1-2;", INPUT_MODE),
            //new InterpreterTestCase("SUB", "1.1-2.2;", INPUT_MODE),
            //new InterpreterTestCase("SUB_ASSIGN", "var a int=1;a-=2;a;", INPUT_MODE),
            //new InterpreterTestCase("DEC", "var i int=1;--i;", INPUT_MODE),
            //new InterpreterTestCase("ADD", "1+2;", INPUT_MODE),
            //new InterpreterTestCase("ADD", "1.1+2.2;", INPUT_MODE),
            //new InterpreterTestCase("ADD_ASSIGN", "var a int=1;a+=2;a;", INPUT_MODE),
            //new InterpreterTestCase("INC", "var i int=1;++i;", INPUT_MODE),
            //new InterpreterTestCase("ASSIGN", "var a int;a=1;a;", INPUT_MODE),
            //new InterpreterTestCase("ASSIGN", "var a int=1;var b int=2;a=b;a;", INPUT_MODE),
            //new InterpreterTestCase("EQ", "1==1;", INPUT_MODE),
            //new InterpreterTestCase("EQ", "var a int=1;a==1;", INPUT_MODE),
            //new InterpreterTestCase("BIT_OR", "0b10 | 0b11;", INPUT_MODE),
            //new InterpreterTestCase("OR", "true || false;", INPUT_MODE),
            //new InterpreterTestCase("OR", "var a bool=true;b var b bool=false;a || b;", INPUT_MODE),
            //new InterpreterTestCase("LT", "1<1;", INPUT_MODE),
            //new InterpreterTestCase("LT", "var a int=1;a<1;", INPUT_MODE),
            //new InterpreterTestCase("LTE", "1<=1;", INPUT_MODE),
            //new InterpreterTestCase("LTE", "var a int=1;a<=1;", INPUT_MODE),
            //new InterpreterTestCase("GT", "1>1;", INPUT_MODE),
            //new InterpreterTestCase("GT", "var a int=1;a>1;", INPUT_MODE),
            //new InterpreterTestCase("GTE", "1>=1;", INPUT_MODE),
            //new InterpreterTestCase("GTE", "var a int=1;a>=1;", INPUT_MODE),
            //new InterpreterTestCase("DIV", "1/2;", INPUT_MODE),
            //new InterpreterTestCase("DIV", "1.1/2.2;", INPUT_MODE),
            //new InterpreterTestCase("DIV_ASSIGN", "var a int=1;a/=2;a;", INPUT_MODE),

            //new InterpreterTestCase("ADD_SUB", "1+2-3;", INPUT_MODE),
            //new InterpreterTestCase("ADD_MUL", "1+2*3;", INPUT_MODE),
            //new InterpreterTestCase("MUL_DIV", "1*2/3;", INPUT_MODE),

            //关键字
            //new InterpreterTestCase("BOOL_VALUE", "true;", INPUT_MODE),
            //new InterpreterTestCase("BOOL_VALUE", "false;", INPUT_MODE),
            //new InterpreterTestCase("BOOL_TYPE", "var a bool;a;", INPUT_MODE),
            //new InterpreterTestCase("BOOL_TYPE", "var a bool=true;a;", INPUT_MODE),
            //new InterpreterTestCase("INT_VALUE", "10;", INPUT_MODE),
            //new InterpreterTestCase("INT_VALUE", "0b10;", INPUT_MODE),
            //new InterpreterTestCase("INT_VALUE", "010;", INPUT_MODE),
            //new InterpreterTestCase("INT_VALUE", "0x10;", INPUT_MODE),
            //new InterpreterTestCase("INT_TYPE", "var a int;a;", INPUT_MODE),
            //new InterpreterTestCase("INT_TYPE", "var a int=2;a;", INPUT_MODE),
            //new InterpreterTestCase("FLOAT_VALUE", "3.00;", INPUT_MODE),
            //new InterpreterTestCase("FLOAT_VALUE", "3.14;", INPUT_MODE),
            //new InterpreterTestCase("FLOAT_TYPE", "var a float;a;", INPUT_MODE),
            //new InterpreterTestCase("FLOAT_TYPE", "var a float=3.14;a;", INPUT_MODE),
            //new InterpreterTestCase("STRING_TYPE", "var a string;a;", INPUT_MODE),
            //new InterpreterTestCase("STRING_TYPE", "var a string=\"a\";a;", INPUT_MODE),
            //new InterpreterTestCase("IF_HKNI", "var a int=1;var b int;if(a==1){b=1;}b;", INPUT_MODE),
            //new InterpreterTestCase("IF_HKNI", "var a int=1;var b int;if(a==1){b=1;}else{b=2;}b;", INPUT_MODE),
            //new InterpreterTestCase("IF_HKNI", "var a int=1;var b int;if(a==2){b=1;}else{b=2;}b;", INPUT_MODE),
            //new InterpreterTestCase("IF_HKNI", "var a int=1;var b int;var c int=1;if(a==1 && c==1){b=1;}b;", INPUT_MODE),
            //new InterpreterTestCase("FUNCTION_HKNI", "function hello(){} hello();", INPUT_MODE),
            //new InterpreterTestCase("BUILTIN_FUNC", "println(\"aaa\");", INPUT_MODE),
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
            Interpreter *p7interpreter = new Interpreter();

            //初始化内置函数
            BuiltinFunc *builtinFunctions = new BuiltinFunc();
            p7interpreter->registerBuiltinFunctions(builtinFunctions);

            //初始化程序最外面那层的环境（作用域）
            Environment *p7env = new Environment();

            Object *p7obj = p7interpreter->DoInterpret(p7program, p7env);
            if (p7obj != nullptr) {
                cout << p7obj->ToString() << endl;
            }

            delete p7interpreter;
        }

        delete p7program;
        delete p7parser;
        delete p7lexer;
    }
}

int main() {
    testInterpreterDoInterpreter();
}
