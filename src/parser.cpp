#include "../include/parser.h"

#include <iostream>
#include <memory>
#include <optional>

Parser::Parser(std::vector<Token> *t) {
    m_tokens = t;
    m_index = 0;
}

using stmt_list = std::vector<std::unique_ptr<stmt_node>>;

stmt_list Parser::parse_PROG() { return parse_STMT_LIST(); }

Token Parser::scan_token() {
    if (m_index >= m_tokens->size()) {
        std::cerr << "PARSE ERROR: End of Input" << std::endl;
        exit(1);
    }

    return m_tokens->at(m_index++);
}

std::unique_ptr<expr_node> Parser::parse_EXPR() {
    return parse_LEXPR();
}

std::unique_ptr<expr_node> Parser::parse_LEXPR() {
    auto left = parse_CEXPR();
    return parse_LEXPR_R(std::move(left));
}

std::unique_ptr<expr_node> Parser::parse_LEXPR_R(std::unique_ptr<expr_node> left) {
    while (peek_type() == TokenType::_LAND || peek_type() == TokenType::_LOR) {
        Token op = scan_token();
        auto right = parse_CEXPR();
        left = std::make_unique<binary_expr_node>(op.type, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<expr_node> Parser::parse_CEXPR() {
    auto left = parse_AEXPR();

    return parse_CEXPR_R(std::move(left));
}

std::unique_ptr<expr_node> Parser::parse_CEXPR_R(std::unique_ptr<expr_node> left) {
    while (peek_type() == TokenType::_LT || peek_type() == TokenType::_GT || peek_type() == TokenType::_LE ||
           peek_type() == TokenType::_GE || peek_type() == TokenType::_EQ || peek_type() == TokenType::_NE) {
        Token op = scan_token();
        auto right = parse_AEXPR();

        left = std::make_unique<binary_expr_node>(op.type, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<expr_node> Parser::parse_AEXPR() {
    auto left = parse_TERM();

    return parse_AEXPR_R(std::move(left));
}

std::unique_ptr<expr_node> Parser::parse_AEXPR_R(std::unique_ptr<expr_node> left) {
    while (peek_type() == TokenType::_PLUS || peek_type() == TokenType::_MINUS) {
        Token op = scan_token();
        auto right = parse_TERM();

        left = std::make_unique<binary_expr_node>(op.type, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<expr_node> Parser::parse_TERM() {
    auto left = parse_FACTOR();
    return parse_TERM_R(std::move(left));
}

std::unique_ptr<expr_node> Parser::parse_TERM_R(std::unique_ptr<expr_node> left) {
    while (peek_type() == TokenType::_MULT || peek_type() == TokenType::_DIV) {
        Token op = scan_token();
        auto right = parse_FACTOR();

        left = std::make_unique<binary_expr_node>(op.type, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<expr_node> Parser::parse_FACTOR() {
    return parse_UNARY();
}

std::unique_ptr<expr_node> Parser::parse_UNARY() {
    Token t = scan_token();

    if (t.type == TokenType::_MINUS || t.type == TokenType::_NOT) {
        return std::make_unique<unary_expr_node>(t.type, std::move(parse_UNARY()));
    }

    if (t.type == TokenType::INT_LIT) {
        return std::make_unique<int_literal_node>(stoi(t.contents));
    }

    if (t.type == TokenType::_IDENTIFIER) {
        return std::make_unique<ident_node>(t.contents);
    }

    if (t.type == TokenType::LPAREN) {
        auto _expr = parse_EXPR();
        if (peek_type() == TokenType::RPAREN) {
            expect_token(TokenType::RPAREN);
            return _expr;
        }
    }

    std::cerr << "PARSE ERROR:\nExpected Term Node" << std::endl;
    exit(1);
}

std::unique_ptr<stmt_node> Parser::parse_STMT() {
    Token t = scan_token();
    if (t.type == TokenType::_KILL) {
        expect_token(TokenType::LPAREN);
        std::unique_ptr<expr_node> value = parse_EXPR();
        expect_token(TokenType::RPAREN);
        expect_token(TokenType::_SEMICOLON);

        return std::make_unique<killstmt_node>(std::move(value));
    } else if (t.type == TokenType::_LET) {
        t = expect_token(TokenType::_IDENTIFIER);
        std::string name = t.contents;
        expect_token(TokenType::_EQUALS);
        std::unique_ptr<expr_node> value = parse_EXPR();
        expect_token(TokenType::_SEMICOLON);

        return std::make_unique<decl_stmt_node>(std::move(name), std::move(value));
    } else if (t.type == TokenType::_IDENTIFIER) {
        expect_token(TokenType::_EQUALS);
        std::unique_ptr<expr_node> value = parse_EXPR();
        expect_token(TokenType::_SEMICOLON);
        return std::make_unique<assign_stmt_node>(std::move(t.contents), std::move(value));
    } else if (t.type == TokenType::_PRINT) {
        expect_token(TokenType::LPAREN);
        std::unique_ptr<expr_node> value = parse_EXPR();
        expect_token(TokenType::RPAREN);
        expect_token(TokenType::_SEMICOLON);
        return std::make_unique<print_stmt_node>(std::move(value));
    } else {
        std::cerr << "PARSE ERROR:\nExpected Kill or Let Tokens" << std::endl;
        exit(1);
    }
}

stmt_list Parser::parse_STMT_LIST() {
    stmt_list list;

    if (peek_type() != TokenType::_KILL && peek_type() != TokenType::_LET && peek_type() != TokenType::_PRINT) {
        std::cerr << "Error: Expected Statement" << std::endl;
        exit(EXIT_FAILURE);
    }

    while (peek_type() == TokenType::_KILL || peek_type() == TokenType::_LET || peek_type() == TokenType::_IDENTIFIER || peek_type() == TokenType::_PRINT) {
        list.push_back(parse_STMT());
    }

    if (peek_type() != TokenType::_EOF) {
        std::cerr << "Unexpected/Unidentified Token Detected" << std::endl;
        exit(EXIT_FAILURE);
    }

    return list;
}

Token Parser::expect_token(const TokenType t) {
    Token st = scan_token();
    if (st.type != t) {
        std::cerr << "PARSE ERROR:\nExpected Token " << t << std::endl;
        exit(1);
    }
    return st;
}

std::optional<TokenType> Parser::peek_type() {
    if (m_index >= m_tokens->size()) {
        return std::nullopt;
    }

    return m_tokens->at(m_index).type;
}
