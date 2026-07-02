#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "Token.h"

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    std::string source;
    
    std::vector<Token> tokens; // Stores the final list of generated tokens

    
    //  Adding internal state tracking variables:
   
    int start = 0;   // Points to the first character of the word being read
    int current = 0; // Points to the specific character we are looking at right now
    int line = 1;    // Tracks the current line number for error messages

    
    //Declare helper methods for character scanning:
   
    
    bool isAtEnd();                // Checks if we have reached the end of the file
    char advance();                // Moves forward one character and returns it
    bool match(char expected);     // Checks the next character and consumes it only if it matches
    char peek();                   // Looks at the current character without moving forward
    void addToken(TokenType type); // Packages the word into a Token and saves it in 'tokens'
};

#endif // LEXER_H
