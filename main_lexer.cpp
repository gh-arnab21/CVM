#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "lexer/Lexer.h"

// Helper function to print a TokenType as a human-readable string
std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::STAR: return "STAR";
        case TokenType::SLASH: return "SLASH";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::LESS: return "LESS";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::LET: return "LET";
        case TokenType::PRINT: return "PRINT";
        case TokenType::INPUT: return "INPUT";
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::WHILE: return "WHILE";
        case TokenType::TRUE_KW: return "TRUE_KW";
        case TokenType::FALSE_KW: return "FALSE_KW";
        case TokenType::END_OF_FILE: return "END_OF_FILE";
        default: return "UNKNOWN";
    }
}

void runLexer(const std::string& sourceCode) {
    Lexer lexer(sourceCode);
    std::vector<Token> tokens = lexer.tokenize();

    std::cout << "\n--- Lexer Output ---" << std::endl;
    for (const Token& token : tokens) {
        std::cout << "[Line " << token.line << "] " 
                  << tokenTypeToString(token.type) 
                  << " : '" << token.lexeme << "'" << std::endl;
    }
    std::cout << "--------------------\n" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        // Read file
        std::ifstream file(argv[1]);
        if (!file.is_open()) {
            std::cerr << "Could not open file." << std::endl;
            return 1;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        runLexer(buffer.str());
    } else {
        // Interactive Lexer
        std::cout << "Lexer Interactive Mode. Type code (Ctrl+C to exit):" << std::endl;
        std::string line;
        while (true) {
            std::cout << "> ";
            if (!std::getline(std::cin, line)) break;
            if (line.empty()) continue;
            runLexer(line);
        }
    }
    return 0;
}
