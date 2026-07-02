#include "Lexer.h"
#include <cctype> // helps in quick check

Lexer::Lexer(const std::string& source) : source(source) {
    
    // Initialize internal state pointers (start, current, line).

    start = 0;   // Set our start finger to 0
    current = 0; // Set our reading finger to 0
    line = 1;    // We always start on line 1
}

std::vector<Token> Lexer::tokenize() {
 
    //  Implement the main tokenization loop.

    while (!isAtEnd()) {             // Keep reading until we run out of text
        start = current;             // Mark the beginning of the next word we are reading
        
        // Inside scanToken() (which you need to implement), you should:
        // 1. Switch on the current character.
        // 2. Handle single-character operators (+, -, *, /).
        // 3. Handle multi-character operators (==, <=).
        // 4. Ignore whitespace and comments.
        // 5. Handle string/number literals by delegating to helper functions (e.g., number(), identifier()).
        // 6. Check if an identifier is actually a keyword (let, print, if, etc.).

        char c = advance();          // Grab the next character and move our finger forward

        switch (c) {
            // SINGLE CHARACTER OPERATORS
            case '(': addToken(TokenType::LEFT_PAREN); break;  
            case ')': addToken(TokenType::RIGHT_PAREN); break; 
            case '{': addToken(TokenType::LEFT_BRACE); break;  
            case '}': addToken(TokenType::RIGHT_BRACE); break; 
            case '+': addToken(TokenType::PLUS); break;        
            case '-': addToken(TokenType::MINUS); break;       
            case '*': addToken(TokenType::STAR); break;        
            case ';': addToken(TokenType::SEMICOLON); break;   
            
            // MULTI-CHARACTER OPERATORS
            case '=': 
                addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); // Check if it's '==' or just '='
                break;
            case '<': 
                addToken(TokenType::LESS); // Found a '<'
                break;
            case '/': 
                if (match('/')) { // If it's a comment `//`
                    while (peek() != '\n' && !isAtEnd()) advance(); // Skip everything until the end of the line
                } else {
                    addToken(TokenType::SLASH); // Otherwise it's just a division sign '/'
                }
                break;
                
            // WHITESPACE (Ignore these completely)
            case ' ':  // Ignore space
            case '\r': // Ignore carriage return
            case '\t': // Ignore tab
                break;
            case '\n': // Handle newline
                line++; // Increment our line counter!
                break;
                
            // NUMBERS AND IDENTIFIERS
            default:
                if (isdigit(c)) {
                    // It's a number!
                    while (isdigit(peek())) advance(); // Keep reading digits until they stop
                    addToken(TokenType::NUMBER);       // Save it as a NUMBER token
                } 
                else if (isalpha(c)) {
                    // It's a word! (Like 'let' or 'myAge')
                    while (isalnum(peek())) advance(); // Keep reading letters and numbers
                    
                    std::string text = source.substr(start, current - start); // Extract the exact word from the text
                    
                    // Check if the word is a special keyword!
                    if (text == "let") addToken(TokenType::LET);
                    else if (text == "print") addToken(TokenType::PRINT);
                    else if (text == "input") addToken(TokenType::INPUT);
                    else if (text == "if") addToken(TokenType::IF);
                    else if (text == "else") addToken(TokenType::ELSE);
                    else if (text == "while") addToken(TokenType::WHILE);
                    else if (text == "true") addToken(TokenType::TRUE_KW);
                    else if (text == "false") addToken(TokenType::FALSE_KW);
                    else addToken(TokenType::IDENTIFIER); // If it's not a keyword, it must be a custom variable name!
                }
                break;
        }
    }
    
    tokens.push_back(Token{TokenType::END_OF_FILE, "", line}); // Add the final EOF token when we finish reading the file

    return tokens;
}


// HELPER METHODS (The Robotic Arms)


bool Lexer::isAtEnd() {
    return current >= source.length(); // Returns true if we hit the end of the text
}

char Lexer::advance() {
    return source[current++]; // Returns the character we are on, and moves finger forward
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;                   // Can't match if we are at the end
    if (source[current] != expected) return false; // Doesn't match what we expected
    current++;                                     // Match found! Move finger forward
    return true;
}

char Lexer::peek() {
    if (isAtEnd()) return '\0'; // Return an empty byte if we are at the end
    return source[current];     // Just look at the character, don't move the finger
}

void Lexer::addToken(TokenType type) {
    std::string text = source.substr(start, current - start); // Cut out the exact word we just read
    tokens.push_back(Token{type, text, line});                // Package it and put it in our 'tokens' box
}
