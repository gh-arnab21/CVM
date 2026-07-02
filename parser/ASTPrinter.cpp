#include "ASTPrinter.h"
#include <iostream>

// Helper to push the text to the right for nice tree formatting
void ASTPrinter::printIndent() {
    for (int i = 0; i < indentLevel; ++i) {
        std::cout << "  "; // Two spaces per indent level
    }
}

// The main entry point
void ASTPrinter::print(ASTNode* node) {
    if (node) {
        node->accept(*this); // Start the visitation!
        std::cout << std::endl;
    }
}

// ---------------------------------------------------------
// EXPRESSIONS (Math & Values)
// ---------------------------------------------------------

void ASTPrinter::visit(BinaryExpr* expr) {
    // Format: (+ 5 5)
    std::cout << "(" << expr->op.lexeme << " ";
    expr->left->accept(*this);  // Traverse left branch
    std::cout << " ";
    expr->right->accept(*this); // Traverse right branch
    std::cout << ")";
}

void ASTPrinter::visit(LiteralExpr* expr) {
    // Just print the raw value (e.g., "42")
    std::cout << expr->value.lexeme;
}

void ASTPrinter::visit(VariableExpr* expr) {
    // Just print the variable name (e.g., "x")
    std::cout << expr->name.lexeme;
}

void ASTPrinter::visit(AssignExpr* expr) {
    std::cout << "(assign " << expr->name.lexeme << " = ";
    expr->value->accept(*this);
    std::cout << ")";
}

void ASTPrinter::visit(InputExpr* expr) {
    std::cout << "(input)";
}


// STATEMENTS (Actions)


void ASTPrinter::visit(PrintStmt* stmt) {
    printIndent();
    std::cout << "(print ";
    stmt->expression->accept(*this);
    std::cout << ")" << std::endl;
}

void ASTPrinter::visit(VarDecl* stmt) {
    printIndent();
    std::cout << "(let " << stmt->name.lexeme;
    if (stmt->initializer) {
        std::cout << " = ";
        stmt->initializer->accept(*this);
    }
    std::cout << ")" << std::endl;
}

void ASTPrinter::visit(BlockStmt* stmt) {
    printIndent();
    std::cout << "(block" << std::endl;
    
    indentLevel++; // Push the branches further to the right!
    for (auto& s : stmt->statements) {
        s->accept(*this);
    }
    indentLevel--; // Pull back to the left
    
    printIndent();
    std::cout << ")" << std::endl;
}

void ASTPrinter::visit(IfStmt* stmt) {
    printIndent();
    std::cout << "(if ";
    stmt->condition->accept(*this);
    std::cout << std::endl;
    
    indentLevel++;
    stmt->thenBranch->accept(*this);
    if (stmt->elseBranch) {
        printIndent();
        std::cout << "(else" << std::endl;
        stmt->elseBranch->accept(*this);
    }
    indentLevel--;
    
    printIndent();
    std::cout << ")" << std::endl;
}

void ASTPrinter::visit(WhileStmt* stmt) {
    printIndent();
    std::cout << "(while ";
    stmt->condition->accept(*this);
    std::cout << std::endl;
    
    indentLevel++;
    stmt->body->accept(*this);
    indentLevel--;
    
    printIndent();
    std::cout << ")" << std::endl;
}
