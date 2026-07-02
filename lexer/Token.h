#ifndef TOKEN_H
#define TOKEN_H

#include <string>

// creating TOKENS(Vocabulary)

enum class TokenType {
    // Single-character tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    PLUS, MINUS, STAR, SLASH, EQUAL, LESS, SEMICOLON,

    // Two-character tokens
    EQUAL_EQUAL,

    // Literals
    IDENTIFIER, NUMBER,

    // Keywords
    LET, PRINT, INPUT, IF, ELSE, WHILE, TRUE_KW, FALSE_KW,

    // Special
    END_OF_FILE
};

//Token structure
struct Token {
    TokenType type;
    std::string lexeme;
    int line;  // for error reporting  
};

#endif // TOKEN_H
