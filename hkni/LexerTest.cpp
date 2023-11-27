#include <iostream>
#include <map>
#include "Lexer.h"
#include "Token.h"

using namespace std;

class C5LexerTestCase {
public:
    string name;
    string input;
    int mode;
public:
    C5LexerTestCase(string name, string input, int mode) {
        this->name = name;
        this->input = input;
        this->mode = mode;
    }

    ~C5LexerTestCase() = default;
};

void testLexerReadNextChar() {
    C5LexerTestCase *tcList[] = {
            //            new C5LexerTestCase("var", "var a=1;", INPUT_MODE),
            new C5LexerTestCase("file", "D:\\workspace\\demo-cpp\\hkni\\lexer_test.txt", FILE_MODE),
    };

    int tcListLen = sizeof(tcList) / sizeof(tcList[0]);
    for (int i = 0; i < tcListLen; i++) {
        Lexer *p7lexer = new Lexer(tcList[i]->input, tcList[i]->mode);

        string t4str;
        char t4char;
        while (true) {
            p7lexer->ReadNextChar();
            t4char = p7lexer->GetReadChar();
            t4str.append(1, t4char);
            if (t4char == 0) {
                break;
            }
        }

        cout << t4str << endl;
        cout << "row:" << p7lexer->GetNowRow() << endl;
        cout << "column:" << p7lexer->GetNowColumn() << endl;

        delete p7lexer;

        cout << "test case:" << tcList[i]->name << " done.\n" << endl;
    }
}

void testLexerGetNextToken() {
    C5LexerTestCase *tcList[] = {
            new C5LexerTestCase("var", "var a=1;", INPUT_MODE),
            //            new C5LexerTestCase("file", "D:\\workspace\\demo-cpp\\hkni\\lexer_test.txt", FILE_MODE),
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
    //    testLexerReadNextChar();
    testLexerGetNextToken();
}
