#ifndef AST_H
#define AST_H

#include <memory>
#include <vector>
#include "../lexer/Token.h"

// Forward declarations so classes know about each other
class ASTVisitor;
class BinaryExpr;
class LiteralExpr;
class VariableExpr;
class PrintStmt;
class VarDecl;
class BlockStmt;
class IfStmt;
class WhileStmt;

// IMPLEMENTING THE VISITOR PATTERN(A professional  way of designing compilers)(good designing)
//let's say u have a number of nodes inorder to do a thing suppose print tree or compile or find error u would have to edit each of the node this creates a mess
// rather u make all the nodes dumb that is no logic only data and then create a visitor class for the required purpose

class ASTNode {
public:
    virtual ~ASTNode() = default;  //virtual destructor in order to prevent memory leaks
    
    // Every node must accept a visitor
    virtual void accept(ASTVisitor& visitor) = 0;
};


// Define specific AST Node classes inheriting from ASTNode.


// --- EXPRESSION NODES (Things that produce a value, like 5 + 5) ---

class BinaryExpr : public ASTNode {
public:
    std::unique_ptr<ASTNode> left;  // The left side of the math equation (e.g., the '1' in 1 + 2)
    Token op;                       // The math operator (e.g., the '+' in 1 + 2)
    std::unique_ptr<ASTNode> right; // The right side of the math equation (e.g., the '2' in 1 + 2)

    BinaryExpr(std::unique_ptr<ASTNode> left, Token op, std::unique_ptr<ASTNode> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}           //constructor and using move because single owner

    void accept(ASTVisitor& visitor) override; // Allows the visitor to enter this node
};

class LiteralExpr : public ASTNode {
public:
    Token value; // Holds a raw value, like the number '42'

    LiteralExpr(Token value) : value(value) {}

    void accept(ASTVisitor& visitor) override; // Allows the visitor to enter this node
};

class VariableExpr : public ASTNode {
public:
    Token name; // Holds the name of the variable we want to use (e.g., 'myScore')

    VariableExpr(Token name) : name(name) {}

    void accept(ASTVisitor& visitor) override; // Allows the visitor to enter this node
};

// Represents updating an existing variable (e.g., 'x = 20')
class AssignExpr : public ASTNode {
public:
    Token name; // The variable being updated (e.g., 'x')
    std::unique_ptr<ASTNode> value; // The new value to store (e.g., '20')

    AssignExpr(Token name, std::unique_ptr<ASTNode> value) 
        : name(name), value(std::move(value)) {}

    void accept(ASTVisitor& visitor) override;
};

// Represents the 'input' keyword (which waits for user input and yields a value)
class InputExpr : public ASTNode {
public:
    InputExpr() = default;

    void accept(ASTVisitor& visitor) override;
};

// --- STATEMENT NODES (Actions that DO something, like 'print x;') ---

class PrintStmt : public ASTNode {
public:
    std::unique_ptr<ASTNode> expression; // The thing we want to print out to the screen

    PrintStmt(std::unique_ptr<ASTNode> expression) : expression(std::move(expression)) {}

    void accept(ASTVisitor& visitor) override; // Allows the visitor to enter this node
};

class VarDecl : public ASTNode {
public:
    Token name;                          // The name of our new variable
    std::unique_ptr<ASTNode> initializer; // The value we are assigning to it (can be null!)

    VarDecl(Token name, std::unique_ptr<ASTNode> initializer)
        : name(name), initializer(std::move(initializer)) {}

    void accept(ASTVisitor& visitor) override; // Allows the visitor to enter this node
};

class BlockStmt : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> statements; // A list of statements grouped inside { }

    BlockStmt(std::vector<std::unique_ptr<ASTNode>> statements)
        : statements(std::move(statements)) {}

    void accept(ASTVisitor& visitor) override; // Allows the visitor to enter this node
};

class IfStmt : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition;  // The test (e.g., x < 10)
    std::unique_ptr<ASTNode> thenBranch; // What to run if the test is true
    std::unique_ptr<ASTNode> elseBranch; // What to run if false (can be null!)

    IfStmt(std::unique_ptr<ASTNode> condition, std::unique_ptr<ASTNode> thenBranch, std::unique_ptr<ASTNode> elseBranch)
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}

    void accept(ASTVisitor& visitor) override; // Allows the visitor to enter this node
};

class WhileStmt : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition; // The test that keeps the loop running
    std::unique_ptr<ASTNode> body;      // The code inside the loop to run

    WhileStmt(std::unique_ptr<ASTNode> condition, std::unique_ptr<ASTNode> body)
        : condition(std::move(condition)), body(std::move(body)) {}

    void accept(ASTVisitor& visitor) override; // Allows the visitor to enter this node
};

// structure for the Visitor
class ASTVisitor {
public:
    virtual void visit(BinaryExpr* expr) = 0; 
    virtual void visit(LiteralExpr* expr) = 0; 
    virtual void visit(VariableExpr* expr) = 0; 
    virtual void visit(AssignExpr* expr) = 0; 
    virtual void visit(InputExpr* expr) = 0;
                                                            // a description for a visitor that it must write how to handle every single type of node
                                                            // we did this in order to prevent crashing of compiler
    virtual void visit(PrintStmt* stmt) = 0; 
    virtual void visit(VarDecl* stmt) = 0; 
    virtual void visit(BlockStmt* stmt) = 0; 
    virtual void visit(IfStmt* stmt) = 0;  
    virtual void visit(WhileStmt* stmt) = 0; 
    
    virtual ~ASTVisitor() = default;
};


// INLINE IMPLEMENTATIONS
// We put these at the bottom so they know what 'ASTVisitor' is

//the forward declarations don;t work here because we are not only declaring the accept function rather we are doing something inside it 

// the below line means ""Here I am! Read my data and do your job!""

inline void BinaryExpr::accept(ASTVisitor& visitor) { visitor.visit(this); }
inline void LiteralExpr::accept(ASTVisitor& visitor) { visitor.visit(this); }
inline void VariableExpr::accept(ASTVisitor& visitor) { visitor.visit(this); }
inline void AssignExpr::accept(ASTVisitor& visitor) { visitor.visit(this); }
inline void InputExpr::accept(ASTVisitor& visitor) { visitor.visit(this); }
inline void PrintStmt::accept(ASTVisitor& visitor) { visitor.visit(this); }
inline void VarDecl::accept(ASTVisitor& visitor) { visitor.visit(this); }
inline void BlockStmt::accept(ASTVisitor& visitor) { visitor.visit(this); }
inline void IfStmt::accept(ASTVisitor& visitor) { visitor.visit(this); }
inline void WhileStmt::accept(ASTVisitor& visitor) { visitor.visit(this); }

#endif // AST_H
