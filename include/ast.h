#pragma once
#include "types.h"
#include <iostream>
#include <memory>
#include <unordered_map>

struct expr_node {
    virtual ~expr_node() = default;
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
    explicit binary_expr_node(TokenType o, std::unique_ptr<expr_node> l,
                              std::unique_ptr<expr_node> r)
        : _operator(o), left(std::move(l)), right(std::move(r)) {}
};

inline void codegen_expr_node(std::ostream &out,
                              std::unique_ptr<expr_node> &expr,
                              std::unordered_map<std::string, int> &var_table) {

    if (auto int_node = dynamic_cast<int_literal_node *>(expr.get())) {
        out << "    mov rax, " << int_node->value << std::endl;
        return;
    }

    if (auto ident = dynamic_cast<ident_node *>(expr.get())) {
        if (var_table.find(ident->name) == var_table.end()) {
            std::cerr << "Variable " << ident->name << " not declared"
                      << std::endl;
            exit(EXIT_FAILURE);
        }

        int ident_offset = -8 * (var_table.at(ident->name) + 1);
        out << "    mov rax, [rbp " << ident_offset << "]\n";
        return;
    }

    if (auto bin = dynamic_cast<binary_expr_node *>(expr.get())) {
        codegen_expr_node(out, bin->right, var_table);
        out << "    PUSH rax\n";

        codegen_expr_node(out, bin->left, var_table);
        out << "    POP rbx\n";
        if (bin->_operator == TokenType::_PLUS) {
            out << "    ADD rax, rbx\n";
        } else if (bin->_operator == TokenType::_MINUS) {
            out << "    SUB rax, rbx\n";
        } else if (bin->_operator == TokenType::_MULT) {
            out << "    IMUL rax, rbx\n";
        } else if (bin->_operator == TokenType::_DIV) {
            out << "    CQO\n";
            out << "    IDIV rbx\n";
        }
    }
}

struct stmt_node {
    virtual ~stmt_node() = default;
    virtual void codegen(std::ostream &out,
                         std::unordered_map<std::string, int> &var_table,
                         int &var_count) = 0;
};

struct killstmt_node : stmt_node {
    std::unique_ptr<expr_node> expr;

    explicit killstmt_node(std::unique_ptr<expr_node> e) : expr(std::move(e)) {}

    void codegen(std::ostream &out,
                 std::unordered_map<std::string, int> &var_table,
                 int &var_count) override {

        codegen_expr_node(out, expr, var_table);

#ifdef _WIN32
        out << "    mov rcx, rax\n";
        out << "    call ExitProcess\n";
#else
        out << "    mov rdi, rax\n";
        out << "    mov rax, 60\n";
        out << "    SYSCALL\n";
#endif
    }
};

struct decl_stmt_node : stmt_node {
    std::string name;
    std::unique_ptr<expr_node> expr;

    explicit decl_stmt_node(std::string n, std::unique_ptr<expr_node> e)
        : name(std::move(n)), expr(std::move(e)) {}

    void codegen(std::ostream &out,
                 std::unordered_map<std::string, int> &var_table,
                 int &var_count) override {
        int offset = -8 * (var_count + 1);
        if (var_table.find(name) != var_table.end()) {
            std::cerr << "Variable " << name << " is being redefined"
                      << std::endl;
            exit(1);
        }
        var_table[name] = var_count;
        var_count++;

        codegen_expr_node(out, expr, var_table);
        out << "    mov [rbp " << offset << "], rax\n";
    }
};

struct assign_stmt_node : stmt_node {
    std::string name;
    std::unique_ptr<expr_node> expr;

    explicit assign_stmt_node(std::string n, std::unique_ptr<expr_node> e)
        : name(std::move(n)), expr(std::move(e)) {}

    void codegen(std::ostream &out,
                 std::unordered_map<std::string, int> &var_table,
                 int &var_count) override {

        if (var_table.find(name) == var_table.end()) {
            std::cerr << "Variable " << name << " is not defined" << std::endl;
            exit(1);
        }

        int slot = var_table[name];
        int offset = -8 * (slot + 1);

        codegen_expr_node(out, expr, var_table);
        out << "    mov [rbp " << offset << "], rax\n";
    }
};

struct print_stmt_node : stmt_node {
    std::unique_ptr<expr_node> expr;

    explicit print_stmt_node(std::unique_ptr<expr_node> e)
        : expr(std::move(e)) {}

    void codegen(std::ostream &out,
                 std::unordered_map<std::string, int> &var_table,
                 int &var_count) override {
        codegen_expr_node(out, expr, var_table);
        out << "    CALL print_int\n";
    }
};
