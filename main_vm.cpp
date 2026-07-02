#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "compiler/Compiler.h"
#include "vm/VM.h"

void runVM(VM& vm, const std::string& sourceCode) {
    // 1. Lex
    Lexer lexer(sourceCode);
    std::vector<Token> tokens = lexer.tokenize();

    // 2. Parse
    Parser parser(tokens);
    auto ast = parser.parse();

    // 3. Compile
    if (ast) {
        Compiler compiler;
        Chunk chunk = compiler.compile(ast.get());
        
        // 4. Execute silently! Only the user's explicit 'print' statements will show up.
        vm.interpret(&chunk);
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
        
        VM vm;
        runVM(vm, buffer.str());
    } else {
        // Interactive Mode
        std::cout << "VM Interactive Mode. Type code (Ctrl+C to exit):" << std::endl;
        std::string line;
        
        VM vm; // Persist the VM so variables are remembered!
        
        while (true) {
            std::cout << "> ";
            if (!std::getline(std::cin, line)) break;
            if (line.empty()) continue;
            runVM(vm, line);
        }
    }
    return 0;
}
