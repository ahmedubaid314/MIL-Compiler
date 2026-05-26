#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "include/ast/stmt_node.h"
#include "include/error_reporter.h"
#include "include/parser.h"
#include "include/scanner.h"
#include "include/scope_handler.h"
#include "include/types.h"

namespace fs = std::filesystem;

int count_declarations(const std::vector<std::unique_ptr<stmt_node>> &stmts) {
    int count = 0;
    for (auto &stmt : stmts) {
        if (dynamic_cast<decl_stmt_node *>(stmt.get())) {
            count++;
        } else if (auto block_stmt = dynamic_cast<block_stmt_node *>(stmt.get())) {
            count += count_declarations(block_stmt->stmt_list);
        }
    }
    return count;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cerr << "EXPECTED: ./compiler <valid MIL file>" << std::endl;
        std::cerr << "Please pass a MIL file" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".mil") {
        std::cerr << "Error: Expected a .mil file" << std::endl;
        exit(1);
    }

    std::ifstream file(filename);

    std::stringstream out;
    out << file.rdbuf();

    std::string src = out.str();

    std::vector<std::string> source_lines;
    std::istringstream stream(src);
    std::string line;
    while (getline(stream, line)) {
        source_lines.push_back(line);
    }

    file.close();

    Scanner scanner;

    std::vector<Token> tokenList = scanner.scan_src(src);

    ErrorReporter reporter;
    reporter.set_source(&source_lines);

    Parser parser(&tokenList, reporter);

    using stmt_list = std::vector<std::unique_ptr<stmt_node>>;
    stmt_list ast = parser.parse_PROG();

    int stack_size = count_declarations(ast);

    stack_size *= 8;

#ifdef _WIN32
    int total_stack = stack_size + 32;
    if (total_stack % 16 != 0) {
        total_stack += 16 - (total_stack % 16);
    }
#endif

    fs::create_directory("target");

    std::ofstream asm_file("target/prog.asm");

#ifdef _WIN32
    asm_file << "extern ExitProcess\n";
#else
    asm_file << "extern print_int\n";
#endif

    asm_file << "global _start\nsection .text\n_start:\n";
    asm_file << "   push rbp\n   mov rbp, rsp\n";

#ifdef _WIN32
    asm_file << "   sub rsp, " << total_stack << std::endl;
#else
    asm_file << "   sub rsp, " << stack_size << std::endl;
#endif

    label_counter lb_count;
    scope_handler scope_;
    scope_.add_scope();

    for (auto &stmt : ast) {

        stmt->codegen(asm_file, scope_, lb_count);
    }
    scope_.remove_scope();
    asm_file << "ret";
    asm_file.close();

#ifdef _WIN32
    int ret1 = system("nasm -f win64 runtime/windows/print_int_windows.asm -o "
                      "build/print_int.obj");
    if (ret1 != 0) {
        std::cerr << "NASM assembling of Print Int failed" << std::endl;
        exit(1);
    }
#else
    int ret1 = system(
        "nasm -felf64 runtime/linux/print_int_linux.asm -o build/print_int.o");
    if (ret1 != 0) {
        std::cout << "NASM assembling of Print Int failed" << std::endl;
        exit(1);
    }
#endif

#ifdef _WIN32
    int ret = system("nasm -f win64 target/prog.asm");
    if (ret != 0) {
        std::cerr << "NASM assembling failed" << std::endl;
        exit(1);
    }

    ret = system("GoLink /console /entry _start target/prog.obj user32.dll "
                 "kernel32.dll");
    if (ret != 0) {
        std::cerr << "Linking Failed" << std::endl;
        exit(1);
    }

#else
    // Linux Code for ELF64 files
    int ret = system("nasm -felf64 target/prog.asm -o target/prog.o");
    if (ret != 0) {
        std::cerr << "NASM assembling failed" << std::endl;
        exit(1);
    }

    ret = system("ld target/prog.o build/print_int.o -o target/prog");
    if (ret != 0) {
        std::cerr << "Linking Failed" << std::endl;
        exit(1);
    }

#endif

    return 0;
}
