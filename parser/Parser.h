#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <memory>
#include <string>
#include <initializer_list>
#include "../lexer/Token.h"
#include "AST.h"

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    
    
    std::unique_ptr<ASTNode> parse();  // kind of the single button to start the entire  parsing process like turning the tokens into 2-D AST tree

private:
    std::vector<Token> tokens;
    
    
    //  Add internal state tracking:
    
    
    int current = 0; // The 'finger' pointing at the current token we are looking at that is to be parsed
    
   // following recursive descent
    // TODO 2: Declare grammar rule methods (Recursive Descent).
    // These methods correspond to your language grammar:
   
    std::vector<std::unique_ptr<ASTNode>> program();
    std::unique_ptr<ASTNode> declaration();
    std::unique_ptr<ASTNode> statement();
    
    std::unique_ptr<ASTNode> expression();
    std::unique_ptr<ASTNode> assignment();
    std::unique_ptr<ASTNode> equality();
    std::unique_ptr<ASTNode> comparison();
    std::unique_ptr<ASTNode> term();
    std::unique_ptr<ASTNode> factor();
    std::unique_ptr<ASTNode> primary();

   
    //  Declare helper methods: to help the parse() build out the branches of the tree
    
    // Checks if the current Token matches what we want. If yes, it grabs it and moves forward!
    bool match(std::initializer_list<TokenType> types);   // use of the feature if initializer_list in order to prevent from writing  that if conditons for all the several tokens
    
    // Just peeks at the current Token to see if it matches, but DOES NOT move forward.
    bool check(TokenType type);
    
    // Grabs the current Token to use it, and moves the finger forward by 1.
    Token advance();
    
    // Checks if we have hit the very end of the file.
    bool isAtEnd();
    
    // The strict teacher! It demands a specific Token (like a ';'). If it's missing, it crashes the compiler!
    Token consume(TokenType type, const std::string& message);
    
    // Looks at the Token the finger is currently pointing at.
    Token peek();
    
    // Looks at the Token we just finished processing (the one behind the finger).
    Token previous();
};

#endif // PARSER_H
