#ifndef COMPILER_H
#define COMPILER_H

#include "../parser/AST.h"
#include "../common/Chunk.h"
#include "../vm/Opcode.h" // We need to know what instructions we can emit!

// The Compiler acts as a "Visitor" that walks down the 2D Abstract Syntax Tree.
// As it visits each node, it emits raw 1D Bytecode (OpCodes) into a Chunk.
class Compiler : public ASTVisitor {
    //this compiler is basically a visitor
public:
    Compiler();
    
    //this is the function used by main.cpp
    //it takes the root of the AST and returns a fully packaged Chunk
    Chunk compile(ASTNode* ast);

    
    // The Visitor Methods (Translating AST Nodes to Bytecode)
   // aa must if u created a visitor need to handle all the nodes of the tree
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
    // The current Chunk we are writing bytecode and constants into
    Chunk currentChunk;
    
    // Helper methods to make writing raw bytes easier
    void emitByte(uint8_t byte);
    void emitBytes(uint8_t byte1, uint8_t byte2);
    
    // Helper method to write a value into the vault, and emit the OP_CONSTANT instruction
    void emitConstant(Value value);

    // Control Flow Helpers
    int emitJump(uint8_t instruction);
    void patchJump(int offset);
    void emitLoop(int loopStart);
};

#endif // COMPILER_H
