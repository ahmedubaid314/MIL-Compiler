#pragma once

#include "types.h"
#include <string>
#include <vector>

#define DFA_MAX_STATES 5
#define DFA_MAX_ALPHABET 81

class Scanner {
  public:
    Scanner();

    // scans the source code and returns a list of tokens
    std::vector<Token> scan_src(const std::string &src) const;

  private:
    int m_dfa[DFA_MAX_STATES][DFA_MAX_ALPHABET];

    size_t index;

    // maps a character to get the transition index on the DFA
    int map_char(const char c) const;

    // returns true if the state is an accept state
    bool is_accept_state(const int state) const;

    // identifies the token type based on extracted string
    TokenType getType(const std::string &accept_state,
                      const int last_accept_state) const;

    // checks if extracted string is a particular token
    TokenType isKeyword(const std::string &identifier) const;

    bool fitsInt32(const std::string &number) const;
};
