#include <iostream>
#include <map>
#include "Lexer.h"
#include "Token.h"

using namespace std;

class C5ParserTestCase {
public:
    string name;
    string input;
    int mode;
public:
    C5ParserTestCase(string name, string input, int mode) {
        this->name = name;
        this->input = input;
        this->mode = mode;
    }

    ~C5ParserTestCase() = default;
};

int main() {
}
