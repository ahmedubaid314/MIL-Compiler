#pragma once
#include <string>

enum class TokenType {
    _KILL,
    LPAREN,
    RPAREN,
    INT_LIT,
    _SEMICOLON,
    _EQUALS,
    _IDENTIFIER,
    _LET,
    _PLUS,
    _MINUS,
    _MULT,
    _DIV,
    _LT,
    _GT,
    _LE,
    _GE,
    _EQ,
    _NE,
    _PRINT,
    _EOF
};

struct Token {
    TokenType type;
    std::string contents; // stores data if required, else NULL
};
