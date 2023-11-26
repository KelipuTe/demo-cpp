cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 17)

add_executable(hello_world hello_world.cpp)

add_executable(
        lexer_test
        hkni/Token.h
        hkni/Lexer.h
        hkni/LexerTest.cpp
)

add_executable(
        parser_test
        hkni/Token.h
        hkni/Lexer.h
        hkni/Parser.h
        hkni/ParserTest.cpp
)