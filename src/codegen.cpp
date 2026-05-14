#include "../include/ast/expr_node.h"
#include "../include/ast/stmt_node.h"

expr_node::~expr_node() {}
stmt_node::~stmt_node() {}

void codegen_expr_node(std::ostream &out, std::unique_ptr<expr_node> &expr, std::unordered_map<std::string, int> &var_table) {

    if (auto int_node = dynamic_cast<int_literal_node *>(expr.get())) {
        out << "    mov rax, " << int_node->value << std::endl;
        return;
    }

    if (auto ident = dynamic_cast<ident_node *>(expr.get())) {
        if (var_table.find(ident->name) == var_table.end()) {
            std::cerr << "Variable " << ident->name << " not declared" << std::endl;
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

// -- KILL STATEMENT NODE --
void killstmt_node::codegen(std::ostream &out, std::unordered_map<std::string, int> &var_table, int &var_count) {

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

// -- DECLARE STATEMENT NODE --

void decl_stmt_node::codegen(std::ostream &out, std::unordered_map<std::string, int> &var_table, int &var_count) {
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

// -- ASSIGN STATEMENT NODE --

void assign_stmt_node::codegen(std::ostream &out, std::unordered_map<std::string, int> &var_table, int &var_count) {

    if (var_table.find(name) == var_table.end()) {
        std::cerr << "Variable " << name << " is not defined" << std::endl;
        exit(1);
    }

    int slot = var_table[name];
    int offset = -8 * (slot + 1);

    codegen_expr_node(out, expr, var_table);
    out << "    mov [rbp " << offset << "], rax\n";
}

// -- PRINT STATEMENT NODE --

void print_stmt_node::codegen(std::ostream &out, std::unordered_map<std::string, int> &var_table, int &var_count) {
    codegen_expr_node(out, expr, var_table);
    out << "    CALL print_int\n";
}
