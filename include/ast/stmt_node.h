#pragma once
#include "expr_node.h"
#include <iostream>

struct stmt_node {
    virtual ~stmt_node();
    virtual void codegen(std::ostream &out, std::unordered_map<std::string, int> &var_table, int &var_count) = 0;
};

struct killstmt_node : stmt_node {
    std::unique_ptr<expr_node> expr;

    explicit killstmt_node(std::unique_ptr<expr_node> e) : expr(std::move(e)) {}
    void codegen(std::ostream &out, std::unordered_map<std::string, int> &var_table, int &var_count) override;
};

struct decl_stmt_node : stmt_node {
    std::string name;
    std::unique_ptr<expr_node> expr;

    explicit decl_stmt_node(std::string n, std::unique_ptr<expr_node> e) : name(std::move(n)), expr(std::move(e)) {}
    void codegen(std::ostream &out, std::unordered_map<std::string, int> &var_table, int &var_count) override;
};

struct assign_stmt_node : stmt_node {
    std::string name;
    std::unique_ptr<expr_node> expr;

    explicit assign_stmt_node(std::string n, std::unique_ptr<expr_node> e) : name(std::move(n)), expr(std::move(e)) {}
    void codegen(std::ostream &out, std::unordered_map<std::string, int> &var_table, int &var_count) override;
};

struct print_stmt_node : stmt_node {
    std::unique_ptr<expr_node> expr;

    explicit print_stmt_node(std::unique_ptr<expr_node> e) : expr(std::move(e)) {}
    void codegen(std::ostream &out, std::unordered_map<std::string, int> &var_table, int &var_count) override;
};
