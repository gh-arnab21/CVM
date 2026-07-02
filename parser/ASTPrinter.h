#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "AST.h"

// ASTPrinter implements the ASTVisitor interface to "visit" every node in the tree.
// Its job is to take a 2D tree and print it out to the terminal in a clean, readable format.
class ASTPrinter : public ASTVisitor {
public:
    // The "Start" button for the printer.  
    // Pass it the root of your tree, and it will print the entire thing!
    void print(ASTNode* node);

    // ---------------------------------------------------------
    // The Visitor Methods 
    // (These are automatically called when we visit a specific type of node)
    // ---------------------------------------------------------
    void visit(BinaryExpr* expr) override;
    void visit(LiteralExpr* expr) override;
    void visit(VariableExpr* expr) override;
    void visit(AssignExpr* expr) override;
    void visit(InputExpr* expr) override;
    void visit(PrintStmt* stmt) override;
    void visit(VarDecl* stmt) override;
    void visit(BlockStmt* stmt) override;
    void visit(IfStmt* stmt) override;
    void visit(WhileStmt* stmt) override;

private:
    int indentLevel = 0; // Keeps track of how far right to push the text for formatting
    void printIndent();  // Helper to print spaces
};

#endif // AST_PRINTER_H
