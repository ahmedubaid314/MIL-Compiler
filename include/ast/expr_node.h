#pragma once
#include "../types.h"
#include <memory>
#include <unordered_map>

struct expr_node {
    virtual ~expr_node();
};

struct int_literal_node : expr_node {
    int value;

    explicit int_literal_node(int v) : value(v) {}
};

struct ident_node : expr_node {
    std::string name;

    explicit ident_node(std::string n) : name(std::move(n)) {}
};

struct binary_expr_node : expr_node {
    TokenType _operator;
    std::unique_ptr<expr_node> left, right;
    explicit binary_expr_node(TokenType o, std::unique_ptr<expr_node> l, std::unique_ptr<expr_node> r) : _operator(o), left(std::move(l)), right(std::move(r)) {}
};

void codegen_expr_node(std::ostream &out, std::unique_ptr<expr_node> &expr, std::unordered_map<std::string, int> &var_table, label_counter &lb_count);
