// Entry point for CVM++
// This file should handle the overall execution flow.

#include <iostream>
#include <string>
#include <iomanip> // For pretty printing
#include <fstream>
#include <sstream>

// Include all the core modules of the CVM++ pipeline
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "parser/ASTPrinter.h"
#include "compiler/Compiler.h"
#include "vm/VM.h"

//Helper to beautifully print the bytecode
void disassembleChunk(const Chunk& chunk) {
    std::cout << "--- BYTECODE CHUNK ---" << std::endl;
    for (size_t i = 0; i < chunk.code.size(); i++) {
        std::cout << std::setw(4) << std::setfill('0') << i << "  ";
        uint8_t instruction = chunk.code[i];
        
        switch (static_cast<Opcode>(instruction)) {
            case Opcode::OP_CONSTANT: {
                uint8_t constantIndex = chunk.code[++i]; // The next byte is the index!
                std::cout << "OP_CONSTANT      " << (int)constantIndex << " (Value: ";
                const Value& val = chunk.constants[constantIndex];
                if (val.isInt()) std::cout << std::get<int>(val.as);
                else if (val.isBool()) std::cout << (std::get<bool>(val.as) ? "true" : "false");
                else if (val.isString()) std::cout << "'" << std::get<std::string>(val.as) << "'";
                std::cout << ")" << std::endl;
                break;
            }
            case Opcode::OP_TRUE: std::cout << "OP_TRUE" << std::endl; break;
            case Opcode::OP_FALSE: std::cout << "OP_FALSE" << std::endl; break;
            case Opcode::OP_ADD: std::cout << "OP_ADD" << std::endl; break;
            case Opcode::OP_SUB: std::cout << "OP_SUB" << std::endl; break;
            case Opcode::OP_MUL: std::cout << "OP_MUL" << std::endl; break;
            case Opcode::OP_DIV: std::cout << "OP_DIV" << std::endl; break;
            case Opcode::OP_EQUAL: std::cout << "OP_EQUAL" << std::endl; break;
            case Opcode::OP_LESS: std::cout << "OP_LESS" << std::endl; break;
            
            case Opcode::OP_JUMP: {
                uint8_t high = chunk.code[++i];
                uint8_t low = chunk.code[++i];
                uint16_t distance = (high << 8) | low;
                std::cout << "OP_JUMP          +" << distance << " -> " << (i + 1 + distance) << std::endl;
                break;
            }
            case Opcode::OP_JUMP_IF_FALSE: {
                uint8_t high = chunk.code[++i];
                uint8_t low = chunk.code[++i];
                uint16_t distance = (high << 8) | low;
                std::cout << "OP_JUMP_IF_FALSE +" << distance << " -> " << (i + 1 + distance) << std::endl;
                break;
            }
            case Opcode::OP_LOOP: {
                uint8_t high = chunk.code[++i];
                uint8_t low = chunk.code[++i];
                uint16_t distance = (high << 8) | low;
                std::cout << "OP_LOOP          -" << distance << " -> " << (i + 1 - distance) << std::endl;
                break;
            }
            
            case Opcode::OP_PRINT: std::cout << "OP_PRINT" << std::endl; break;
            case Opcode::OP_INPUT: std::cout << "OP_INPUT" << std::endl; break;
            case Opcode::OP_DEFINE_GLOBAL: {
                uint8_t index = chunk.code[++i];
                std::cout << "OP_DEFINE_GLOBAL " << (int)index << " (Name: '" << std::get<std::string>(chunk.constants[index].as) << "')" << std::endl;
                break;
            }
            case Opcode::OP_GET_GLOBAL: {
                uint8_t index = chunk.code[++i];
                std::cout << "OP_GET_GLOBAL    " << (int)index << " (Name: '" << std::get<std::string>(chunk.constants[index].as) << "')" << std::endl;
                break;
            }
            case Opcode::OP_SET_GLOBAL: {
                uint8_t index = chunk.code[++i];
                std::cout << "OP_SET_GLOBAL    " << (int)index << " (Name: '" << std::get<std::string>(chunk.constants[index].as) << "')" << std::endl;
                break;
            }
            case Opcode::OP_RETURN: std::cout << "OP_RETURN" << std::endl; break;
            default: std::cout << "Unknown Opcode: " << (int)instruction << std::endl; break;
        }
    }
    std::cout << "----------------------" << std::endl;
}

// We added this helper to test our new ASTPrinter AND Compiler!
void runCode(VM& vm, const std::string& sourceCode) {
    std::cout << "\n--- Source Code ---" << std::endl;
    std::cout << sourceCode<< std::endl;
    std::cout << "-------------------\n" << std::endl;

    // 1. The Lexer chops the string into Tokens
    Lexer lexer(sourceCode);
    auto tokens = lexer.tokenize();

    // 2. The Parser builds the 2D Tree
    Parser parser(tokens);
    auto ast = parser.parse();

    if (ast) {
        // 3. Print the Tree so we can visually check it
        std::cout << "--- AST Tree ---" << std::endl;
        ASTPrinter printer;
        printer.print(ast.get());
        std::cout << "----------------\n" << std::endl;

        // 4. Compile the Tree into flat Bytecode!
        Compiler compiler;
        Chunk chunk = compiler.compile(ast.get());

        // 5. Disassemble the chunk to visually prove the bytecode is correct!
        disassembleChunk(chunk);

        // 6. Execute the Bytecode using our Virtual Machine!
        std::cout << "\n--- VM Output ---" << std::endl;
        vm.interpret(&chunk);
        std::cout << "-----------------" << std::endl;
    } else {
        std::cout << "Compilation Failed due to Syntax Error." << std::endl;
    }
}
//FILE MODE
void runFile(const char* path) {
    // Open the file
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        exit(74); // Standard exit code for IO error
    }

    // Read the entire file into a string
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string sourceCode = buffer.str();

    // Create a VM and run it!
    VM vm;
    runCode(vm, sourceCode);
}
// INTERACTIVE MODE (REPL)
void runPrompt() {
    std::string line;
    std::cout << "Welcome to CVM++ Interactive REPL!" << std::endl;
    std::cout << "Type your code below (Ctrl+C to exit)." << std::endl;
    
    // Create ONE Virtual Machine for the entire session!
    // This allows it to remember variables between lines.
    VM vm;

    // Loop infinitely, asking for code
    while (true) {
        std::cout << "> ";
        
        // Read a line from the user
        if (!std::getline(std::cin, line)) {
            std::cout << std::endl;
            break; // Exit if they hit EOF (Ctrl+D/Ctrl+Z)
        }
        
        // If the line is empty, skip it
        if (line.empty()) continue;
        
        // Run the single line of code!
        runCode(vm, line);
    }
}

int main(int argc, char* argv[]) {
  
    // The user decides in the terminal which mode he/she wants to operate in 
   
    
    if (argc == 1) {
        // If they just run '.\cvm.exe', start the Interactive Prompt
        runPrompt();
    } 
    else if (argc == 2) {
        // If they run '.\cvm.exe script.cvm', run the file
        runFile(argv[1]);
    } 
    else {
        // Too many arguments
        std::cout << "Usage: cvm [path to script]" << std::endl;
        return 64; // Standard exit code for usage error
    }

    return 0;
}
