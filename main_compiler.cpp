#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "compiler/Compiler.h"

// We bring in the disassembler to nicely print the bytecode
void disassembleChunk(const Chunk& chunk) {
    std::cout << "\n--- BYTECODE CHUNK ---" << std::endl;
    for (size_t i = 0; i < chunk.code.size(); i++) {
        std::cout << std::setw(4) << std::setfill('0') << i << "  ";
        uint8_t instruction = chunk.code[i];
        
        switch (static_cast<Opcode>(instruction)) {
            case Opcode::OP_CONSTANT: {
                uint8_t constantIndex = chunk.code[++i]; 
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
    std::cout << "----------------------\n" << std::endl;
}

void runCompiler(const std::string& sourceCode) {
    // 1. Lex
    Lexer lexer(sourceCode);
    std::vector<Token> tokens = lexer.tokenize();

    // 2. Parse
    Parser parser(tokens);
    auto ast = parser.parse();

    // 3. Compile & Print Bytecode!
    if (ast) {
        Compiler compiler;
        Chunk chunk = compiler.compile(ast.get());
        disassembleChunk(chunk);
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
        runCompiler(buffer.str());
    } else {
        // Interactive Mode
        std::cout << "Compiler Interactive Mode. Type code (Ctrl+C to exit):" << std::endl;
        std::string line;
        while (true) {
            std::cout << "> ";
            if (!std::getline(std::cin, line)) break;
            if (line.empty()) continue;
            runCompiler(line);
        }
    }
    return 0;
}
