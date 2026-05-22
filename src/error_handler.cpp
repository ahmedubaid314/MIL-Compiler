#include "../include/error_reporter.h"

void ErrorReporter::expected_token(Token recieved, TokenType expected) {

    if (expected == TokenType::_SEMICOLON) {
        int line_to_show = recieved.line_number - 1;

        if (line_to_show < 0)
            line_to_show = 0;

        if (line_to_show >= (int)source_lines->size())
            line_to_show = source_lines->size() - 1;

        std::cerr << "Missing Semicolon ;" << std::endl;
        std::cerr << "Error on Line Number " << recieved.line_number - 1 << std::endl;
        std::cerr << "\t" << source_lines->at(recieved.line_number - 1) << std::endl;
        exit(1);
    }

    std::cerr << "Error on Line Number " << recieved.line_number + 1 << std::endl;
    std::cerr << recieved.line_number + 1 << "\t" << source_lines->at(recieved.line_number) << std::endl;
    std::cerr << "\t";
    for (size_t i = 0; i < recieved.column; i++)
        std::cerr << " ";
    std::cerr << "^" << std::endl;
    std::cerr << "Expected " << expected << " but got " << recieved.type << std::endl;

    exit(1);
}

void ErrorReporter::unexpected_token(Token recieved) {

    std::cerr << "Error on Line Number " << recieved.line_number + 1 << std::endl;
    std::cerr << recieved.line_number + 1 << "\t" << source_lines->at(recieved.line_number) << std::endl;
    std::cerr << "\t";
    for (size_t i = 0; i < recieved.column; i++)
        std::cerr << " ";
    std::cerr << "^" << std::endl;
    std::cerr << "Unexpected token \"" << recieved.type << "\" recieved" << std::endl;

    exit(1);
}

void ErrorReporter::redeclared_variable(Token var) {
    std::cerr << "Error on Line Number " << var.line_number + 1 << std::endl;
    std::cerr << var.line_number + 1 << "\t" << source_lines->at(var.line_number) << std::endl;
    std::cerr << "\t";
    for (size_t i = 0; i < var.column; i++)
        std::cerr << " ";
    std::cerr << "^" << std::endl;
    std::cerr << "Variable " << var.contents << " is being redeclared" << std::endl;
}

void ErrorReporter::undefined_var(Token var) {
    std::cerr << "Error on Line Number " << var.line_number + 1 << std::endl;
    std::cerr << var.line_number + 1 << "\t" << source_lines->at(var.line_number) << std::endl;
    std::cerr << "\t";
    for (size_t i = 0; i < var.column; i++)
        std::cerr << " ";
    std::cerr << "^" << std::endl;
    std::cerr << "Variable " << var.contents << " is not declared" << std::endl;
}
