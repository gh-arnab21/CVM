#ifndef OPCODE_H
#define OPCODE_H

#include <cstdint>

// ---------------------------------------------------------
// CVM++ INSTRUCTION SET ARCHITECTURE (ISA)
// This file formally defines the exact bytecode commands 
// (Operation Codes) that our Virtual Machine can execute.
// Every language feature must translate down to these!
// ---------------------------------------------------------

enum class Opcode : uint8_t {
    // --- Constants & Stack Operations ---
    OP_CONSTANT, // Loads a constant from the pool. Followed by 1 byte (index).
    OP_TRUE,     // Optimization: Push boolean 'true' to stack directly.
    OP_FALSE,    // Optimization: Push boolean 'false' to stack directly.
    OP_POP,      // Pops the top value off the stack (used to discard unused expression results).

    // --- Math Operations (Pops 2 values, performs math, pushes result) ---
    OP_ADD,      // a + b
    OP_SUB,      // a - b
    OP_MUL,      // a * b
    OP_DIV,      // a / b
    
    // --- Comparisons (Pops 2 values, compares, pushes boolean result) ---
    OP_EQUAL,    // a == b
    OP_LESS,     // a < b
    OP_NOT,      // Pops 1 value, logically inverts it (!a), pushes result.

    // --- Variables ---
    // (These are typically followed by 1 byte representing the variable name's index in the constant pool)
    OP_DEFINE_GLOBAL, // Creates a new variable (e.g., let x = 10;)
    OP_GET_GLOBAL,    // Reads an existing variable (e.g., print x;)
    OP_SET_GLOBAL,    // Updates an existing variable (e.g., x = 20;)

    // --- Control Flow (Followed by 2 bytes for the jump distance offset) ---
    OP_JUMP,          // Unconditional jump forward (used to skip 'else' blocks).
    OP_JUMP_IF_FALSE, // Pops condition. If false, jumps over the 'if' body.
    OP_LOOP,          // Jumps backwards (used to loop back to the start of a 'while' condition).

    // --- Built-in I/O ---
    OP_PRINT,    // Pops 1 value from the stack and prints it to the terminal.
    OP_INPUT,    // Prompts the user for input, creates a Value, and pushes it to the stack.

    // --- System ---
    OP_RETURN    // Exits the current execution context or halts the script entirely.
};

#endif // OPCODE_H
