#include "../include/scanner.h"
#include <string>

namespace scanner_helpers {
TokenType emit_state_3(const std::string &accept_state) {
    if (accept_state == "(") {
        return TokenType::LPAREN;
    } else if (accept_state == ")") {
        return TokenType::RPAREN;
    } else if (accept_state == ";") {
        return TokenType::_SEMICOLON;
    } else if (accept_state == "=") {
        return TokenType::_EQUALS;
    } else if (accept_state == "+") {
        return TokenType::_PLUS;
    } else if (accept_state == "-") {
        return TokenType::_MINUS;
    } else if (accept_state == "*") {
        return TokenType::_MULT;
    } else if (accept_state == "/") {
        return TokenType::_DIV;
    } else if (accept_state == "<") {
        return TokenType::_LT;
    } else if (accept_state == ">") {
        return TokenType::_GT;
    } else if (accept_state == "!") {
        return TokenType::_NOT;
    } else if (accept_state == "&") {
        return TokenType::_BAND;
    } else if (accept_state == "|") {
        return TokenType::_BOR;
    } else if (accept_state == "^") {
        return TokenType::_XOR;
    } else if (accept_state == "~") {
        return TokenType::_BNOT;
    } else if (accept_state == "{") {
        return TokenType::_LCURLY;
    } else if (accept_state == "}") {
        return TokenType::_RCURLY;
    } else if (accept_state == "$") {
        return TokenType::_SCOPERES;
    }

    std::cerr << "UNIDENTIFIED STATE 3 TOKEN" << std::endl;
    exit(1);
}

TokenType emit_state_4(const std::string &accept_state) {
    if (accept_state == "<=") {
        return TokenType::_LE;
    } else if (accept_state == ">=") {
        return TokenType::_GE;
    } else if (accept_state == "==") {
        return TokenType::_EQ;
    } else if (accept_state == "!=") {
        return TokenType::_NE;
    } else if (accept_state == "&&") {
        return TokenType::_LAND;
    } else if (accept_state == "||") {
        return TokenType::_LOR;
    } else if (accept_state == "<<") {
        return TokenType::_LSHIFT;
    } else if (accept_state == ">>") {
        return TokenType::_RSHIFT;
    }
    std::cerr << "UNIDENTIFIED STATE 4 TOKEN" << std::endl;
    exit(1);
}

} // namespace scanner_helpers

// Public

// Constructor: defines the dfa
Scanner::Scanner() {
    for (int i = 0; i < DFA_MAX_STATES; i++) {
        for (int j = 0; j < DFA_MAX_ALPHABET; j++) {
            m_dfa[i][j] = -1;
        }
    }

    // Identifiers

    for (char c = 'a'; c <= 'z'; c++) {
        m_dfa[0][map_char(c)] = 1;
    }

    for (char c = 'A'; c <= 'Z'; c++) {
        m_dfa[0][map_char(c)] = 1;
    }

    m_dfa[0][map_char('_')] = 1;

    for (int i = 0; i <= 9; i++) {
        m_dfa[1][i] = 1;
    }

    for (char c = 'a'; c <= 'z'; c++) {
        m_dfa[1][map_char(c)] = 1;
    }

    for (char c = 'A'; c <= 'Z'; c++) {
        m_dfa[1][map_char(c)] = 1;
    }

    m_dfa[1][map_char('_')] = 1;

    // INT LITERALS

    for (int i = 0; i <= 9; i++) {
        m_dfa[0][i] = 2;
    }

    for (int i = 0; i <= 9; i++) {
        m_dfa[2][i] = 2;
    }

    // SINGLE CHAR
    m_dfa[0][map_char('(')] = 3;
    m_dfa[0][map_char(')')] = 3;
    m_dfa[0][map_char(';')] = 3;
    m_dfa[0][map_char('=')] = 3;
    m_dfa[0][map_char('+')] = 3;
    m_dfa[0][map_char('-')] = 3;
    m_dfa[0][map_char('*')] = 3;
    m_dfa[0][map_char('/')] = 3;
    m_dfa[0][map_char('<')] = 3;
    m_dfa[0][map_char('>')] = 3;
    m_dfa[0][map_char('!')] = 3;
    m_dfa[0][map_char('&')] = 3;
    m_dfa[0][map_char('|')] = 3;
    m_dfa[0][map_char('^')] = 3;
    m_dfa[0][map_char('~')] = 3;
    m_dfa[0][map_char('{')] = 3;
    m_dfa[0][map_char('}')] = 3;
    m_dfa[0][map_char('$')] = 3;

    // TWO CHAR
    m_dfa[3][map_char('=')] = 4;
    m_dfa[3][map_char('&')] = 4;
    m_dfa[3][map_char('|')] = 4;
    m_dfa[3][map_char('<')] = 4;
    m_dfa[3][map_char('>')] = 4;
}

std::vector<Token> Scanner::scan_src(const std::string &src) const {

    std::vector<Token> token_list;

    size_t index = 0;
    size_t line_tracker = 0;
    size_t line_start_tracker = 0;

    while (index < src.length()) {
        char c = src.at(index);

        // skip whitespaces

        if (c == '\n') {
            line_tracker++;
            line_start_tracker = index + 1;
            index++;
            continue;
        }

        if (std::isspace(c)) {
            index++;
            continue;
        }

        size_t start = index;
        int state = 0;
        int last_accept_state = -1;
        size_t last_accept_index = -1;

        do {
            int transition_index = map_char(c);
            if (transition_index == -1) {
                std::cerr << "Unidentified Character used: " << c << std::endl;
                exit(1);
            }
            int next_state = m_dfa[state][transition_index];
            if (next_state == -1) {
                break;
            }

            state = next_state;
            if (is_accept_state(state)) {
                last_accept_state = state;
                last_accept_index = index;
            }
            index++;
            if (index >= src.length())
                break;
            c = src.at(index);

            if (std::isspace(c))
                break;

        } while (index < src.length());

        if (last_accept_state == -1) {
            std::cerr << "Lexical Error. Failed to Tokenize" << std::endl;
            exit(1);
        }

        std::string current_token = src.substr(
            start, last_accept_index - start + 1); // might need a -1 too

        if (getType(current_token, last_accept_state) == TokenType::INT_LIT &&
            current_token.length() > 1 && current_token.at(0) == '0') {
            std::cerr << "Integers cannot have leading zeroes" << std::endl;
            exit(1);
        }

        if (getType(current_token, last_accept_state) == TokenType::INT_LIT &&
            !fitsInt32(current_token)) {
            std::cerr << "OVERFLOW ERROR: Integer must be a 32bit value"
                      << std::endl;
            exit(1);
        }

        token_list.push_back(
            Token{.type = getType(current_token, last_accept_state),
                  .contents = current_token,
                  .line_number = line_tracker,
                  .column = start - line_start_tracker});
    }

    token_list.push_back(
        Token{.type = TokenType::_EOF, .contents = std::string(), .line_number = line_tracker, .column = 0});

    return token_list;
}

// Private

int Scanner::map_char(char c) const {
    if (c >= '0' && c <= '9')
        return c - '0'; // 0–9

    if (c >= 'a' && c <= 'z')
        return 10 + (c - 'a'); // 10–35

    if (c >= 'A' && c <= 'Z')
        return 36 + (c - 'A'); // 36–61

    switch (c) {
    case '_':
        return 62;
    case '(':
        return 63;
    case ')':
        return 64;
    case ';':
        return 65;
    case '=':
        return 66;
    case '+':
        return 67;
    case '-':
        return 68;
    case '*':
        return 69;
    case '/':
        return 70;
    case '<':
        return 71;
    case '>':
        return 72;
    case '!':
        return 73;
    case '&':
        return 74;
    case '|':
        return 75;
    case '~':
        return 76;
    case '^':
        return 77;
    case '{':
        return 78;
    case '}':
        return 79;
    case '$':
        return 80;
    default:
        return -1;
    }
}

bool Scanner::is_accept_state(const int state) const {
    if (state > 0 && state <= 4) //  1-4 are accept states
        return true;
    else
        return false;
}

TokenType Scanner::getType(const std::string &accept_state,
                           const int last_accept_state) const {
    if (last_accept_state == 2) {
        return TokenType::INT_LIT;
    }

    if (last_accept_state == 1) {
        return isKeyword(accept_state);
    }

    if (last_accept_state == 3) {
        return scanner_helpers::emit_state_3(accept_state);
    }

    if (last_accept_state == 4) {
        return scanner_helpers::emit_state_4(accept_state);
    }

    std::cerr << "UNEXPECTED TOKEN GENERATED" << std::endl;
    exit(1);
}

TokenType Scanner::isKeyword(const std::string &identifier) const {
    if (identifier == "kill") {
        return TokenType::_KILL;
    } else if (identifier == "let") {
        return TokenType::_LET;
    } else if (identifier == "print") {
        return TokenType::_PRINT;
    } else if (identifier == "if") {
        return TokenType::_IF;
    } else if (identifier == "else") {
        return TokenType::_ELSE;
    }

    return TokenType::_IDENTIFIER;
}

bool Scanner::fitsInt32(const std::string &number) const {
    const std::string MAX_VALUE = "2147483647";

    if (number.size() < MAX_VALUE.size())
        return true;
    if (number.size() > MAX_VALUE.size())
        return false;

    return number <= MAX_VALUE;
}
