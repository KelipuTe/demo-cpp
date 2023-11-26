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

void f8TestLexerF8ReadNextChar();

void f8TestLexerF8GetNextToken();

int main() {
    //    f8TestLexerF8ReadNextChar();
    f8TestLexerF8GetNextToken();
}

void f8TestLexerF8ReadNextChar() {
    C5LexerTestCase *a5c5tc[] = {
            //            new C5LexerTestCase("var", "var a=1;", INPUT_MODE),
            new C5LexerTestCase("file", "D:\\workspace\\demo-cpp\\hkni\\lexer_test.txt", FILE_MODE),
    };

    int a5c5tcLen = sizeof(a5c5tc) / sizeof(a5c5tc[0]);
    for (int i = 0; i < a5c5tcLen; i++) {
        cout << a5c5tc[i]->name << " test case start..." << endl;

        string t4str;
        C5Lexer *p7c5l = new C5Lexer(a5c5tc[i]->input, a5c5tc[i]->mode);

        char t4char;
        while (true) {
            p7c5l->F8ReadNextChar();
            t4char = p7c5l->F8GetReadChar();
            t4str.append(1, t4char);
            if (t4char == 0) {
                break;
            }
        }

        cout << t4str << endl;
        cout << "row:" << p7c5l->F8GetNowRow() << endl;
        cout << "column:" << p7c5l->F8GetNowColumn() << endl;

        cout << a5c5tc[i]->name << " test case end." << '\n' << endl;

        delete p7c5l;
    }
}

void f8TestLexerF8GetNextToken() {
    cout << "f8TestLexerF8GetNextToken" << endl;

    C5LexerTestCase *a5c5tc[] = {
            new C5LexerTestCase("var", "var a=1;", INPUT_MODE),
            //            new C5LexerTestCase("file", "D:\\workspace\\demo-cpp\\hkni\\lexer_test.txt", FILE_MODE),
    };

    int a5c5tcLen = sizeof(a5c5tc) / sizeof(a5c5tc[0]);
    for (int i = 0; i < a5c5tcLen; i++) {
        cout << a5c5tc[i]->name << " test case start..." << endl;

        C5Lexer *p7c5l = new C5Lexer(a5c5tc[i]->input, a5c5tc[i]->mode);

        while (true) {
            C5Token c5p7t = p7c5l->F8GetNextToken();
            cout << c5p7t.token_type << ',' << c5p7t.literal << endl;
            if (c5p7t.token_type == END) {
                break;
            }
        }

        cout << a5c5tc[i]->name << " test case end." << '\n' << endl;

        delete p7c5l;
    }
};




