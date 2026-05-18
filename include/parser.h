#pragma once
#include "ast/stmt_node.h"
#include "scanner.h"

#include <memory>
#include <optional>
#include <vector>

class Parser {
  public:
    explicit Parser(std::vector<Token> *t);

    using stmt_list = std::vector<std::unique_ptr<stmt_node>>;
    stmt_list parse_PROG();

  private:
    std::vector<Token> *m_tokens;
    size_t m_index;

    std::unique_ptr<stmt_node> parse_STMT();
    stmt_list parse_STMT_LIST();

    std::unique_ptr<expr_node> parse_EXPR();
    std::unique_ptr<expr_node> parse_LEXPR();
    std::unique_ptr<expr_node> parse_LEXPR_R(std::unique_ptr<expr_node> left);
    std::unique_ptr<expr_node> parse_CEXPR();
    std::unique_ptr<expr_node> parse_CEXPR_R(std::unique_ptr<expr_node> left);
    std::unique_ptr<expr_node> parse_AEXPR();
    std::unique_ptr<expr_node> parse_AEXPR_R(std::unique_ptr<expr_node> left);
    std::unique_ptr<expr_node> parse_TERM();
    std::unique_ptr<expr_node> parse_TERM_R(std::unique_ptr<expr_node> left);
    std::unique_ptr<expr_node> parse_FACTOR();
    std::unique_ptr<expr_node> parse_UNARY();

    Token scan_token();

    Token expect_token(TokenType t);

    std::optional<TokenType> peek_type();
};
