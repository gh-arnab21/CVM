#include "Parser.h"
#include <stdexcept>
#include <iostream>


// CONSTRUCTOR

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {
    current = 0; // When main.cpp creates the Parser, it hands over the tokens vector. We set our current finger to 0 so 
                // we are pointing at the very first token on the conveyor belt.
}


//  HELPER METHODS (Tools to interact with the conveyor belt inorder to build the tree)


Token Parser::peek() {
    if (current >= tokens.size()) {
       
        return Token{TokenType::END_OF_FILE, "", -1}; //returned an object without saving it in a variable
    }
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::END_OF_FILE; // If the token we are looking at is EOF, we are done!
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;  // Just check if it matches, don't move the finger
}

Token Parser::advance() {
    if (!isAtEnd()) current++; // Move the finger forward by 1
    return previous();         // Return the token we just stepped over
}

bool Parser::match(std::initializer_list<TokenType> types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance(); // We found a match! Move the finger forward!
            return true;
        }
    }
    return false; // No match found
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    
    // Grab the token that caused the error so we can tell the user exactly where they messed up!
    Token errorToken = peek();
    
    // If it's the wrong token, the strict teacher throws a massive error WITH the line number!
    throw std::runtime_error("[Line " + std::to_string(errorToken.line) + "] Syntax Error: " + message);
}


// the only function main.cpp cares about. When it wants to build the tree, it calls parse().

std::unique_ptr<ASTNode> Parser::parse() {
    try {
       
        std::vector<std::unique_ptr<ASTNode>> allStatements = program(); // Get all statements until EOF like printstm ,varaiabledclr etc
        
        // combine all into the blockstmnt which is the root of our tree
        return std::make_unique<BlockStmt>(std::move(allStatements));    
        
    } catch (const std::runtime_error& error) {
        // If our strict 'consume()' teacher threw an error, we catch it here and crash gracefully
        std::cerr << "Parser crashed: " << error.what() << std::endl;    // Print the error safely
        return nullptr;                                                  // Stop parsing
    }
}




//  Implement the recursive descent methods here.



//  (Actions that do things)

std::vector<std::unique_ptr<ASTNode>> Parser::program() {
    std::vector<std::unique_ptr<ASTNode>> statements;
                                                         //ONE MIght think what happened to the expression nodes
                                                        // but remember those are tucked inside the statement nodes only hence they are handled as well
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }
    return statements;
}

std::unique_ptr<ASTNode> Parser::declaration() {
    // If the next token is 'let', we are creating a new variable!
    if (match({TokenType::LET})) {
        Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
        
        std::unique_ptr<ASTNode> initializer = nullptr;
        // Check if there is an '=' sign
        if (match({TokenType::EQUAL})) {
            initializer = expression(); // Build the math logic for the value
        }
        
        consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
        return std::make_unique<VarDecl>(name, std::move(initializer));
    }
    
    // If it's not a variable declaration, it must be a normal statement
    return statement();
}

std::unique_ptr<ASTNode> Parser::statement() {
    // 1. Is it a PRINT statement?
    if (match({TokenType::PRINT})) {
        std::unique_ptr<ASTNode> value = expression();
        consume(TokenType::SEMICOLON, "Expect ';' after value.");
        return std::make_unique<PrintStmt>(std::move(value));
    }
    
    // 2. Is it a BLOCK { } of code?
    if (match({TokenType::LEFT_BRACE})) {
        std::vector<std::unique_ptr<ASTNode>> statements;
        // Keep parsing statements until we hit the '}' closing brace
        while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
            statements.push_back(declaration());
        }
        consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
        return std::make_unique<BlockStmt>(std::move(statements));
    }
    
    // 3. Is it an IF statement?
    if (match({TokenType::IF})) {
        consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
        std::unique_ptr<ASTNode> condition = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");
        
        std::unique_ptr<ASTNode> thenBranch = statement();
        std::unique_ptr<ASTNode> elseBranch = nullptr;
        
        if (match({TokenType::ELSE})) { // Optional else branch!
            elseBranch = statement();
        }
        
        return std::make_unique<IfStmt>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
    }
    
    // 4. Is it a WHILE loop?
    if (match({TokenType::WHILE})) {
        consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
        std::unique_ptr<ASTNode> condition = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
        std::unique_ptr<ASTNode> body = statement();
        
        return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
    }

    // 5. If it's none of the above, it's just a raw math expression (like '5 + 5;')
    std::unique_ptr<ASTNode> expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return expr;
}

//  EXPRESSIONS (Math and Logic)
std::unique_ptr<ASTNode> Parser::expression() {
    // The top-level expression just passes down to assignment
    return assignment();
}

std::unique_ptr<ASTNode> Parser::assignment() {
    // Parse the left-hand side first
    std::unique_ptr<ASTNode> expr = equality();

    // If we see an '=', we know we are assigning a value!
    if (match({TokenType::EQUAL})) {
        Token equals = previous(); // The '=' token
        
        // Recursively parse the right-hand side (to allow x = y = 20)
        std::unique_ptr<ASTNode> value = assignment();

        // The left-hand side MUST be a variable (we can't do '5 = 10')
        VariableExpr* varExpr = dynamic_cast<VariableExpr*>(expr.get());
        if (varExpr) {
            Token name = varExpr->name;
            return std::make_unique<AssignExpr>(name, std::move(value));
        }

        // If it's not a variable, we throw an error!
        throw std::runtime_error("Invalid assignment target.");
    }

    return expr;
}

std::unique_ptr<ASTNode> Parser::equality() {
    std::unique_ptr<ASTNode> expr = comparison();                   // suppose 5*5 +5/5 < 5*6 +7/1 

  
    while (match({TokenType::EQUAL_EQUAL})) {                      
        Token op = previous();                                           // ==
        std::unique_ptr<ASTNode> right = comparison();                    // 2<3
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right)); 
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::comparison() {
    std::unique_ptr<ASTNode> expr = term();                      // 5*5 + 5/5  

    
    while (match({TokenType::LESS})) {                           
        Token op = previous();                                  // <      
        std::unique_ptr<ASTNode> right = term();                // 5*6 + 7/1   
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right)); 
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::term() {
    std::unique_ptr<ASTNode> expr = factor();       // 5*5

    while (match({TokenType::MINUS, TokenType::PLUS})) { 
        Token op = previous();                    // +
        std::unique_ptr<ASTNode> right = factor();    // 5/5
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right)); 
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::factor() {
    std::unique_ptr<ASTNode> expr = primary();                     //5

    while (match({TokenType::SLASH, TokenType::STAR})) {       
        Token op = previous();                                            //* 
        std::unique_ptr<ASTNode> right = primary();                    //5
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right)); 
    }
    return expr;
}


std::unique_ptr<ASTNode> Parser::primary() {
    // If it's a number OR a boolean, return a LiteralExpr!
    if (match({TokenType::NUMBER, TokenType::TRUE_KW, TokenType::FALSE_KW})) {
        return std::make_unique<LiteralExpr>(previous());
    }
    // If it's a variable name, return a VariableExpr!
    if (match({TokenType::IDENTIFIER})) {
        return std::make_unique<VariableExpr>(previous());
    }
    // If it's the 'input' keyword, return an InputExpr!
    if (match({TokenType::INPUT})) {
        return std::make_unique<InputExpr>();
    }
    
    // Handle grouping parentheses
    if (match({TokenType::LEFT_PAREN})) {              // 1. Matches '('
        auto expr = expression();                       // 2. Parses whatever is inside recursively
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression."); // 3. Demands closing ')'
        return expr;                                    // 4. Returns the inner tree directly
    }
    
    // If we hit a Token that is NOT a math expression, throw an error!
    throw std::runtime_error("Expect expression.");
}


