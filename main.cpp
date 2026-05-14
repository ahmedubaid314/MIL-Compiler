#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "include/parser.h"
#include "include/scanner.h"

#include <unordered_map>

namespace fs = std::filesystem;

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

    Scanner scanner;

    std::vector<Token> tokenList = scanner.scan_src(src);

    std::cout << "Code was successfully scanned" << std::endl;

    Parser parser(&tokenList);

    using stmt_list = std::vector<std::unique_ptr<stmt_node>>;
    stmt_list ast = parser.parse_PROG();

    int stack_size = 0;
    for (auto &stmt : ast) {
        auto decl_stmt = dynamic_cast<decl_stmt_node *>(stmt.get());
        if (decl_stmt) {
            stack_size++;
        }
    }

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

    std::unordered_map<std::string, int> var_table;
    int var_count = 0;

    for (auto &stmt : ast) {
        stmt->codegen(asm_file, var_table, var_count);
    }
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
