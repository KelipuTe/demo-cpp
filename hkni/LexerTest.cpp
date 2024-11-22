#include <iostream>
#include <map>

#include "Lexer.h"

using namespace std;

using namespace hkni;

class LexerTestCase {
public:
    string name;
    string input;
    LexerMode mode;
public:
    LexerTestCase(string name, string input, LexerMode mode) {
        this->name = name;
        this->input = input;
        this->mode = mode;
    }
};


void testLexerGetNextToken() {
    LexerTestCase *tcList[] = {
            //new LexerTestCase("INPUT_MODE", "var a int = 2;", INPUT_MODE),
            new LexerTestCase("INPUT_MODE", "if(a==2){b=10;}", INPUT_MODE),
            //new LexerTestCase("FILE_MODE", "D:\\workspace\\demo-cpp\\hkni\\test_code.txt", FILE_MODE),
    };

    int tcListLen = sizeof(tcList) / sizeof(tcList[0]);
    for (int i = 0; i < tcListLen; i++) {
        cout << "test case[" << i << "] " << tcList[i]->name << ":" << endl;

        Lexer *p7Lexer = new Lexer(tcList[i]->input, tcList[i]->mode);

        while (true) {
            Token token = p7Lexer->GetNextToken();
            cout << token.TokenType << ',' << token.Literal << endl;
            if (token.TokenType == END) {
                break;
            }
        }

        delete p7Lexer;
    }
};

int main() {
    testLexerGetNextToken();
}