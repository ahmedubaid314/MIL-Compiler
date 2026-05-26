#include "../include/ast/expr_node.h"
#include "../include/ast/stmt_node.h"
#include "../include/error_reporter.h"
#include <memory>
#include <ostream>
#include <string>

namespace expr_helper {

bool is_comparison(TokenType op) {
    return op == TokenType::_LT || op == TokenType::_GT || op == TokenType::_LE || op == TokenType::_GE || op == TokenType::_EQ || op == TokenType::_NE;
}

void emit_comparison(std::ostream &out, TokenType op) {
    out << "    CMP rax, rbx\n";

    std::string set_instr;
    if (op == TokenType::_LT)
        set_instr = "SETL";
    else if (op == TokenType::_GT)
        set_instr = "SETG";
    else if (op == TokenType::_LE)
        set_instr = "SETLE";
    else if (op == TokenType::_GE)
        set_instr = "SETGE";
    else if (op == TokenType::_EQ)
        set_instr = "SETE";
    else if (op == TokenType::_NE)
        set_instr = "SETNE";

    out << "    " << set_instr << " al\n";
    out << "    MOVZX rax, al\n";
}

bool is_arithmetic(TokenType op) {
    return op == TokenType::_PLUS || op == TokenType::_MINUS || op == TokenType::_MULT || op == TokenType::_DIV;
}

void emit_arithmetic(std::ostream &out, TokenType op) {
    if (op == TokenType::_PLUS) {
        out << "    ADD rax, rbx\n";
    } else if (op == TokenType::_MINUS) {
        out << "    SUB rax, rbx\n";
    } else if (op == TokenType::_MULT) {
        out << "    IMUL rax, rbx\n";
    } else if (op == TokenType::_DIV) {
        out << "    CQO\n";
        out << "    IDIV rbx\n";
    }
}

bool is_logical(TokenType op) {
    return op == TokenType::_LAND || op == TokenType::_LOR;
}

void emit_logical(std::ostream &out, TokenType op, std::unique_ptr<expr_node> &right, scope_handler &scope_, label_counter &lb_count) {
    std::string skip_label = "skip_" + std::to_string(lb_count.logical_circuit);
    std::string end_label = "end_" + std::to_string(lb_count.logical_circuit++);

    if (op == TokenType::_LAND) {
        out << "  TEST rax, rax\n";
        out << "  JZ " << skip_label << "\n";
    } else {
        out << "  TEST rax, rax\n";
        out << "  JNZ " << skip_label << "\n";
    }

    codegen_expr_node(out, right, scope_, lb_count);
    out << "  TEST rax, rax\n";
    out << "  SETNZ al\n";
    out << "  MOVZX rax, al\n";
    out << skip_label << ":\n";
    return;
}

bool is_bitwise(TokenType op) {
    return op == TokenType::_BAND || op == TokenType::_BOR || op == TokenType::_XOR || op == TokenType::_LSHIFT || op == TokenType::_RSHIFT;
}

// TODO: Might need to save RCX for loops
void emit_bitwise(std::ostream &out, TokenType op) {

    if (op == TokenType::_BAND) {
        out << "  AND rax, rbx\n";
    } else if (op == TokenType::_BOR) {
        out << "  OR rax, rbx\n";
    } else if (op == TokenType::_XOR) {
        out << "  XOR rax, rbx\n";
    } else if (op == TokenType::_LSHIFT) {
        out << "  MOV cl, bl\n";
        out << "  SHL rax, cl\n";
    } else if (op == TokenType::_RSHIFT) {
        out << "  MOV cl, bl\n";
        out << "  SHR rax, cl\n";
    }
}

} // namespace expr_helper

expr_node::~expr_node() {}
stmt_node::~stmt_node() {}

void codegen_int_litr_node(std::ostream &out, const int_literal_node *int_node) {

    out << "    mov rax, " << int_node->value << std::endl;
    return;
}

void codegen_ident_node(std::ostream &out, const ident_node *ident, scope_handler &scope_) {

    auto offset = scope_.get_var(ident->token.contents, ident->scope_depth);
    if (!offset.has_value()) {
        ErrorReporter::undefined_var(ident->token);
        exit(EXIT_FAILURE);
    }

    int ident_offset = offset.value();
    out << "    mov rax, [rbp " << ident_offset << "]\n";
    return;
}

void codegen_binary_expr_node(std::ostream &out, binary_expr_node *bin, scope_handler &scope_, label_counter &lb_count) {

    if (expr_helper::is_logical(bin->_operator)) {
        codegen_expr_node(out, bin->left, scope_, lb_count);
        expr_helper::emit_logical(out, bin->_operator, bin->right, scope_, lb_count);
        return;
    }

    codegen_expr_node(out, bin->right, scope_, lb_count);
    out << "    PUSH rax\n";

    codegen_expr_node(out, bin->left, scope_, lb_count);
    out << "    POP rbx\n";

    if (expr_helper::is_bitwise(bin->_operator)) {

        expr_helper::emit_bitwise(out, bin->_operator);

    } else if (expr_helper::is_arithmetic(bin->_operator)) {

        expr_helper::emit_arithmetic(out, bin->_operator);

    } else if (expr_helper::is_comparison(bin->_operator)) {

        expr_helper::emit_comparison(out, bin->_operator);
    }
}

void codegen_unary_expr_node(std::ostream &out, unary_expr_node *unary, scope_handler &scope_, label_counter &lb_count) {
    codegen_expr_node(out, unary->unary, scope_, lb_count);

    if (unary->_operator == TokenType::_NOT) {
        out << "  TEST rax, rax\n";
        out << "  SETZ al\n";
        out << "  MOVZX rax, al\n";
    } else if (unary->_operator == TokenType::_MINUS) {
        out << "  NEG rax\n";
    } else if (unary->_operator == TokenType::_BNOT) {
        out << "   NOT rax\n";
    }
}

void codegen_expr_node(std::ostream &out, std::unique_ptr<expr_node> &expr, scope_handler &scope_, label_counter &lb_count) {

    if (auto int_node = dynamic_cast<int_literal_node *>(expr.get())) {
        codegen_int_litr_node(out, int_node);
    }

    if (auto ident = dynamic_cast<ident_node *>(expr.get())) {
        codegen_ident_node(out, ident, scope_);
    }

    if (auto bin = dynamic_cast<binary_expr_node *>(expr.get())) {
        codegen_binary_expr_node(out, bin, scope_, lb_count);
    }

    if (auto unary = dynamic_cast<unary_expr_node *>(expr.get())) {
        codegen_unary_expr_node(out, unary, scope_, lb_count);
    }
}

// -- KILL STATEMENT NODE --
void killstmt_node::codegen(std::ostream &out, scope_handler &scope_, label_counter &lb_count) {

    codegen_expr_node(out, expr, scope_, lb_count);

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

void decl_stmt_node::codegen(std::ostream &out, scope_handler &scope_, label_counter &lb_count) {

    if (scope_.var_exists_curr_scope(token.contents) == true) {
        ErrorReporter::redeclared_variable(token);
        exit(1);
    }
    int offset = scope_.add_var(token.contents);

    codegen_expr_node(out, expr, scope_, lb_count);
    out << "    mov [rbp " << offset << "], rax\n";
}

// -- ASSIGN STATEMENT NODE --

void assign_stmt_node::codegen(std::ostream &out, scope_handler &scope_, label_counter &lb_count) {

    auto o = scope_.get_var(token.contents);
    if (!o.has_value()) {
        ErrorReporter::undefined_var(token);
        exit(1);
    }

    int offset = o.value();

    codegen_expr_node(out, expr, scope_, lb_count);
    out << "    mov [rbp " << offset << "], rax\n";
}

// -- PRINT STATEMENT NODE --

void print_stmt_node::codegen(std::ostream &out, scope_handler &scope_, label_counter &lb_count) {
    codegen_expr_node(out, expr, scope_, lb_count);
    out << "    CALL print_int\n";
}

// -- BLOCK STATEMENT NODE --

void block_stmt_node::codegen(std::ostream &out, scope_handler &scope_, label_counter &lb_count) {
    scope_.add_scope();
    for (auto &stmt : stmt_list) {
        stmt->codegen(out, scope_, lb_count);
    }
    scope_.remove_scope();
}
