#pragma once
#include "types.h"
#include <string>
#include <vector>

class ErrorReporter {
  public:
    ErrorReporter() {}

    inline static void set_source(std::vector<std::string> *s) { source_lines = s; }

    static void expected_token(Token recieved, TokenType expected);
    static void unexpected_token(Token recieved);
    static void redeclared_variable(Token var);
    static void undefined_var(Token var);

  private:
    inline static std::vector<std::string> *source_lines = nullptr;
    void print_error_context();
};
