#pragma once
#include "../error_reporter.h"
#include "expr_node.h"
#include <iostream>
#include <memory>
#include <vector>

struct stmt_node {
    virtual ~stmt_node();
    virtual void codegen(std::ostream &out, std::unordered_map<std::string, int> &var_table, int &var_count, label_counter &lb_count) = 0;
};

struct killstmt_node : stmt_node {
    std::unique_ptr<expr_node> expr;

    explicit killstmt_node(std::unique_ptr<expr_node> e) : expr(std::move(e)) {}
    void codegen(std::ostream &out, std::unordered_map<std::string, int> &var_table, int & /*var_count*/, label_counter &lb_count) override;
};

struct decl_stmt_node : stmt_node {
    Token token;
    std::unique_ptr<expr_node> expr;

    explicit decl_stmt_node(Token t, std::unique_ptr<expr_node> e) : token(t), expr(std::move(e)) {}
    void codegen(std::ostream &out, std::unordered_map<std::string, int> &var_table, int &var_count, label_counter &lb_count) override;
};

struct assign_stmt_node : stmt_node {
    Token token;
    std::unique_ptr<expr_node> expr;

    explicit assign_stmt_node(Token t, std::unique_ptr<expr_node> e) : token(t), expr(std::move(e)) {}
    void codegen(std::ostream &out, std::unordered_map<std::string, int> &var_table, int & /*var_count*/, label_counter &lb_count) override;
};

struct print_stmt_node : stmt_node {
    std::unique_ptr<expr_node> expr;

    explicit print_stmt_node(std::unique_ptr<expr_node> e) : expr(std::move(e)) {}
    void codegen(std::ostream &out, std::unordered_map<std::string, int> &var_table, int & /*var_count*/, label_counter &lb_count) override;
};

struct block_stmt_node : stmt_node {
    std::vector<std::unique_ptr<stmt_node>> stmt_list;

    explicit block_stmt_node(std::vector<std::unique_ptr<stmt_node>> list) : stmt_list(std::move(list)) {}
    void codegen(std::ostream &out, std::unordered_map<std::string, int> &var_table, int & /*var_count*/, label_counter &lb_count) override;
};
