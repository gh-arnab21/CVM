# The Ultimate Beginner's Guide to Building CVM++

Welcome! If you are a complete beginner, building a programming language might sound like black magic. But it is actually just an assembly line in a factory.

Imagine you own a factory. A customer sends you a raw English sentence: `"let x = 10 + 20;"`. 
Your factory has 4 different workers (**Lexer**, **Parser**, **Compiler**, **Virtual Machine**). Each worker does one small job to the sentence and passes it to the next.

Here is the exact order you will follow to build this project, step-by-step.

---

## STEP 1: The Lexer (The Reader)
**Where you will work:** `lexer/Token.h`, `lexer/Lexer.h`, `lexer/Lexer.cpp`

**The Concept:** The computer sees code as one giant string of characters: `'l', 'e', 't', ' ', 'x', ...`
The Lexer's job is to read these individual characters and group them into logical "words" called **Tokens**.

**The Process:**
1. You will give the Lexer the raw string: `"let x = 10 + 20;"`
2. The Lexer will scan it from left to right.
3. It will output a neat list of Tokens:
   - `[KEYWORD_LET]`
   - `[IDENTIFIER: "x"]`
   - `[EQUAL_SIGN]`
   - `[NUMBER: "10"]`
   - `[PLUS_SIGN]`
   - `[NUMBER: "20"]`
   - `[SEMICOLON]`

**Your Job here:** You will write `if/else` rules in `Lexer.cpp`. For example: If you see a `+`, make a `PLUS` token. If you see a number, keep reading until the number ends, and bundle it into a `NUMBER` token.

---

## STEP 2: The Parser (The Grammarian)
**Where you will work:** `parser/AST.h`, `parser/Parser.h`, `parser/Parser.cpp`

**The Concept:** The Lexer gave us a list of words. But words don't have meaning without grammar. The Parser checks if the grammar is correct and organizes the words into a tree structure called an **Abstract Syntax Tree (AST)**.

**The Process:**
1. The Parser looks at the Tokens: `[NUMBER 10]`, `[PLUS]`, `[NUMBER 20]`.
2. It knows grammar rules. It knows that `NUMBER + NUMBER` is a valid mathematical expression.
3. It creates an AST Node. Imagine an upside-down tree:
```text
       (+)
      /   \
    (10) (20)
```

**Your Job here:** You will write functions in `Parser.cpp`. A function called `expression()` will look for numbers and math operators to build these little trees. If the user typed `let x = + 20;`, your Parser will realize the grammar is wrong and throw an error!

---

## STEP 3: The Compiler (The Translator)
**Where you will work:** `compiler/Compiler.h`, `compiler/Compiler.cpp`

**The Concept:** Now we have a beautiful tree. But computers and Virtual Machines don't understand trees. They only understand a flat list of commands called **Bytecode** or **Opcodes** (those `OP_` things!). The Compiler travels through the tree and translates it into Opcodes.

**The Process:**
1. The Compiler looks at the tree: `10 + 20`.
2. It looks at the left side and outputs a command: `OP_CONSTANT 10`
3. It looks at the right side and outputs a command: `OP_CONSTANT 20`
4. It looks at the top `+` and outputs a command: `OP_ADD`
5. The final output is an array of instructions: `[OP_CONSTANT, 10, OP_CONSTANT, 20, OP_ADD]`

**Your Job here:** You will write code in `Compiler.cpp` that "walks" over the AST tree. Every time it steps on a piece of the tree, it will push the corresponding `OP_` code into a list.

---

## STEP 4: The Virtual Machine (The Executioner)
**Where you will work:** `vm/Opcode.h`, `vm/VM.h`, `vm/VM.cpp`, `common/Value.h`

**The Concept:** The VM is a fake computer inside your real computer. It takes the array of commands the Compiler generated and actually executes the math and logic using a **Stack** (like a stack of plates).

**The Process:**
1. The VM creates an empty Stack.
2. It reads `OP_CONSTANT 10`. It puts the number `10` on the stack.
3. It reads `OP_CONSTANT 20`. It puts the number `20` on the stack.
4. It reads `OP_ADD`. It takes the top two plates off the stack (`10` and `20`), adds them together to get `30`, and puts the `30` back on the stack!

**Your Job here:** You will write a giant `switch` statement in `VM.cpp`. 
*   `case OP_ADD:` -> pop two numbers, add them, push the result.
*   `case OP_PRINT:` -> pop a number, print it to the screen.

---

## STEP 5: Tying it all together
**Where you will work:** `main.cpp`

**The Concept:** Turn on the factory conveyor belt!

**The Process:**
1. You read your script file: `scripts/example.cvm`.
2. You pass the text to the **Lexer**.
3. You pass the Lexer's tokens to the **Parser**.
4. You pass the Parser's tree to the **Compiler**.
5. You pass the Compiler's bytecode to the **VM**.
6. The VM runs and prints the result to your screen!

You will build this project exactly in this order: **Step 1 -> Step 2 -> Step 3 -> Step 4**. Good luck!
