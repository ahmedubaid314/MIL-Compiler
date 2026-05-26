#pragma once
#include <iostream>
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
    _LAND,
    _LOR,
    _NOT,
    _XOR,
    _BAND,
    _BOR,
    _BNOT,
    _LSHIFT,
    _RSHIFT,
    _PRINT,
    _LCURLY,
    _RCURLY,
    _SCOPERES,
    _IF,
    _ELSE,
    _EOF
};

inline std::ostream &operator<<(std::ostream &os, TokenType t) {
    switch (t) {
    case TokenType::_KILL:
        return os << "kill";
    case TokenType::LPAREN:
        return os << "'('";
    case TokenType::RPAREN:
        return os << "')'";
    case TokenType::INT_LIT:
        return os << "Integer Literal";
    case TokenType::_SEMICOLON:
        return os << "';'";
    case TokenType::_EQUALS:
        return os << "'='";
    case TokenType::_IDENTIFIER:
        return os << "Identifier";
    case TokenType::_LET:
        return os << "let";
    case TokenType::_PLUS:
        return os << "+";
    case TokenType::_MINUS:
        return os << "-";
    case TokenType::_DIV:
        return os << "/";
    case TokenType::_MULT:
        return os << "*";
    case TokenType::_PRINT:
        return os << "print";
    case TokenType::_LT:
        return os << "<";
    case TokenType::_GT:
        return os << ">";
    case TokenType::_LE:
        return os << "<=";
    case TokenType::_GE:
        return os << ">=";
    case TokenType::_EQ:
        return os << "==";
    case TokenType::_NE:
        return os << "!=";
    case TokenType::_LAND:
        return os << "&&";
    case TokenType::_LOR:
        return os << "||";
    case TokenType::_NOT:
        return os << "!";
    case TokenType::_XOR:
        return os << "^";
    case TokenType::_BAND:
        return os << "&";
    case TokenType::_BOR:
        return os << "|";
    case TokenType::_BNOT:
        return os << "~";
    case TokenType::_LSHIFT:
        return os << "<<";
    case TokenType::_RSHIFT:
        return os << ">>";
    case TokenType::_LCURLY:
        return os << "{";
    case TokenType::_RCURLY:
        return os << "}";
    case TokenType::_SCOPERES:
        return os << "$";
    case TokenType::_IF:
        return os << "if";
    case TokenType::_ELSE:
        return os << "else";
    default:
        return os << "ERROR";
    }
}

struct Token {
    TokenType type;
    std::string contents; // stores data if required, else NULL
    size_t line_number;
    size_t column;
};

struct label_counter {
    int logical_circuit = 0;
    int if_stmt = 0;
};
