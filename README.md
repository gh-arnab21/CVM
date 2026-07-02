# CVM++ (Custom Virtual Machine Plus Plus)

Welcome to **CVM++**, a custom-built, highly optimized, Turing-complete programming language and stack-based Virtual Machine. I wrote this language entirely from scratch in modern C++ (C++17) to explore the depths of compiler theory and systems engineering.

Instead of relying on existing frameworks, this project implements a full end-to-end compiler pipeline. It takes raw source code, breaks it down, understands the grammatical structure, and translates it into blazing-fast 1D bytecode that runs on a custom Virtual CPU.

##  Key Features

*   **Turing Complete Control Flow:** Full support for complex logic like `if/else` conditional branching and `while` loops. The compiler handles this by calculating precise bytecode jump offsets (`OP_JUMP`, `OP_JUMP_IF_FALSE`, `OP_LOOP`) to route execution instantly.
*   **Dynamic Variable Management:** You can easily instantiate global variables (`let x = 10;`) and dynamically update them on the fly (`x = x + 1;`). The Virtual Machine securely manages this state in its internal memory.
*   **Interactive I/O & Smart Parsing:** Output results directly to the console using `print`, or dynamically pause execution to capture user data using the `input` keyword. The VM utilizes "Smart Parsing" to automatically evaluate your input as an integer for math, or securely fall back to a string if you type letters!
*   **Dual Execution Modes:** Choose between a dynamic Read-Eval-Print Loop (REPL) for quick, line-by-line prototyping, or run entire `.cvm` scripts directly from your filesystem.
*   **Custom Instruction Set Architecture (ISA):** The language is powered by a tightly packed, 8-bit Opcode-based instruction set enabling fast and memory-efficient execution.
*   **Diagnostic Tooling Suite:** I built modular diagnostic executables that allow developers to "look under the hood" and visualize every specific step of the pipeline (from raw Lexer Tokens to live CPU Stack traces).

##  How It Works (The Architecture Pipeline)

CVM++ processes code through a strict 4-step pipeline:

1.  **Lexical Analysis (Lexer):** 
    Reads the raw source string character-by-character, catching keywords, literals, and operators, and packages them into discrete, manageable `Token` objects.
2.  **Parsing (Recursive Descent Parser):** 
    Consumes the stream of Tokens according to strict operator precedence and grammar rules, producing a 2D Abstract Syntax Tree (AST). 
3.  **Compilation (Bytecode Compiler):** 
    Traverses the AST using the Visitor Design Pattern. It evaluates nodes, packs constants into a secure memory vault, and flattens the tree into 1D bytecode (`Opcode` instructions) packaged inside a `Chunk`.
4.  **Execution (Virtual Machine):** 
    A highly performant Stack-Based VM that reads the `Chunk` byte-by-byte. It executes math operations, manages variable memory states, calculates 16-bit jump offsets, and processes I/O operations directly on the CPU stack.

##  Build Instructions

This project uses **CMake** for its build system, ensuring smooth cross-platform compatibility.

### 1. Generate the Build Files
Navigate to the root directory of the project and configure CMake:
```bash
cmake -B build -S .
```

### 2. Compile the Project
Build the highly optimized executables. The build is explicitly configured with `-O3`, `-Wall`, `-Wextra`, and `-pedantic` flags to enforce maximum execution speed and rigorous C++ code quality:
```bash
cmake --build build
```

## Running CVM++

Once compiled, you can run CVM++ in two different ways depending on your workflow.

### Mode 1: File Mode (Run a Script)
If you have written a full program and saved it as a file (e.g., `scripts/example.cvm`), you can execute the entire script at once by passing the file path to the executable:

```bash
# On Windows
.\build\cvm.exe scripts\example.cvm

# On Linux/macOS
./build/cvm scripts/example.cvm
```

### Mode 2: Interactive REPL
If you just run the executable by itself, it will launch the interactive REPL. This is perfect for testing snippets, doing math, or watching the pipeline diagnose your code in real-time.

```bash
# On Windows
.\build\cvm.exe

# On Linux/macOS
./build/cvm
```

*Once inside the REPL, try typing a loop!*
```javascript
let count = 0;
while (count < 3) {
    let add = input;
    count = count + add;
    print count;
}
```

### Advanced Diagnostic Tools
If you're curious about how the compiler is interpreting your code, you can run any of the targeted diagnostic executables built alongside the main program:
*   `cvm_lexer`: Visualizes the raw Token stream.
*   `cvm_parser`: Pretty-prints the 2D Abstract Syntax Tree structure.
*   `cvm_compiler`: Disassembles the bytecode and prints the Constant Pool.
*   `cvm_vm`: Traces the live CPU stack during bytecode execution.
*   `cvm_final`: Runs the complete pipeline silently without debug logs (maximum speed).
