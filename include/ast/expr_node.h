#pragma once
#include "../error_reporter.h"
#include "../scope_handler.h"
#include "../types.h"
#include <memory>

struct expr_node {
    virtual ~expr_node();
};

struct int_literal_node : expr_node {
    int value;

    explicit int_literal_node(int v) : value(v) {}
};

struct ident_node : expr_node {
    Token token;
    int scope_depth;

    explicit ident_node(Token t, int depth = 0) : token(t), scope_depth(depth) {}
};

struct binary_expr_node : expr_node {
    TokenType _operator;
    std::unique_ptr<expr_node> left, right;
    explicit binary_expr_node(TokenType o, std::unique_ptr<expr_node> l, std::unique_ptr<expr_node> r) : _operator(o), left(std::move(l)), right(std::move(r)) {}
};

struct unary_expr_node : expr_node {
    TokenType _operator;
    std::unique_ptr<expr_node> unary;
    explicit unary_expr_node(TokenType o, std::unique_ptr<expr_node> u) : _operator(o), unary(std::move(u)) {}
};

void codegen_expr_node(std::ostream &out, std::unique_ptr<expr_node> &expr, scope_handler &scope_, label_counter &lb_count);
void codegen_int_litr_node(std::ostream &out, const int_literal_node *int_node);
void codegen_ident_node(std::ostream &out, const ident_node *ident, scope_handler &scope_);
void codegen_binary_expr_node(std::ostream &out, binary_expr_node *expr, scope_handler &scope_, label_counter &lb_count);
void codegen_unary_expr_node(std::ostream &out, unary_expr_node *unary, scope_handler &scope_, label_counter &lb_count);
