#include <iostream>
#include <map>

#include "Lexer.h"

using namespace std;

class LexerTestCase {
public:
    string name;
    string input;
    LEXER_MODE mode;
public:
    LexerTestCase(string name, string input, LEXER_MODE mode) {
        this->name = name;
        this->input = input;
        this->mode = mode;
    }
};

void testLexerGetNextToken() {
    LexerTestCase *tcList[] = {
            new LexerTestCase("INT", "int a=1;", INPUT_MODE),
            //            new LexerTestCase("file", "D:\\workspace\\demo-cpp\\hkni\\lexer_test.txt", FILE_MODE),
    };

    int tcListLen = sizeof(tcList) / sizeof(tcList[0]);
    for (int i = 0; i < tcListLen; i++) {
        Lexer *p7Lexer = new Lexer(tcList[i]->input, tcList[i]->mode);

        while (true) {
            Token c5p7t = p7Lexer->GetNextToken();
            cout << c5p7t.TokenType << ',' << c5p7t.Literal << endl;
            if (c5p7t.TokenType == END) {
                break;
            }
        }

        delete p7Lexer;

        cout << "test case:" << tcList[i]->name << " done.\n" << endl;
    }
};

int main() {
    testLexerGetNextToken();
}
