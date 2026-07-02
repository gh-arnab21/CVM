#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "parser/ASTPrinter.h"

void runParser(const std::string& sourceCode) {
    // Step 1: Lex the source code into tokens
    Lexer lexer(sourceCode);
    std::vector<Token> tokens = lexer.tokenize();

    // Step 2: Parse the tokens into an Abstract Syntax Tree
    Parser parser(tokens);
    auto ast = parser.parse();

    if (ast) {
        std::cout << "\n--- Parser AST Output ---" << std::endl;
        ASTPrinter printer;
        printer.print(ast.get());
        std::cout << "-------------------------\n" << std::endl;
    } else {
        std::cout << "Syntax Error! Failed to build AST." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        // File Mode
        std::ifstream file(argv[1]);
        if (!file.is_open()) {
            std::cerr << "Could not open file." << std::endl;
            return 1;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        runParser(buffer.str());
    } else {
        // Interactive Mode
        std::cout << "Parser Interactive Mode. Type code (Ctrl+C to exit):" << std::endl;
        std::string line;
        while (true) {
            std::cout << "> ";
            if (!std::getline(std::cin, line)) break;
            if (line.empty()) continue;
            runParser(line);
        }
    }
    return 0;
}
