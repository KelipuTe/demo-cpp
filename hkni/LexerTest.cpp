#include <iostream>

#include "Lexer.h"

using namespace hkni;

//词法分析测试用例
class LexerTestCase {
public:
    string name;
    string input;
    LexerMode mode;
    LexerTestCase(string name, string input, LexerMode mode) {
        this->name = name;
        this->input = input;
        this->mode = mode;
    }
};

void testLexerGetNextToken() {
    LexerTestCase *tcList[] = {
        new LexerTestCase("INPUT_MODE", "if(a==2){b=10;}", INPUT_MODE),
        // new LexerTestCase("FILE_MODE", "D:\\workspace\\demo-cpp\\hkni\\test_code.txt", FILE_MODE),
    };

    int testCaseListLen = sizeof(tcList) / sizeof(tcList[0]);
    for (int i = 0; i < testCaseListLen; i++) {
        cout << "test case[" << i << "] " << tcList[i]->name << ":" << endl;

        auto *lexerP7 = new Lexer(tcList[i]->input, tcList[i]->mode);

        while (true) {
            Token token = lexerP7->GetNextToken();
            cout << token.TokenType << ',' << token.Literal << endl;
            if (token.TokenType == END) {
                break;
            }
        }

        delete lexerP7;
    }
};

int main() {
    testLexerGetNextToken();
}
