#ifndef VM_H
#define VM_H

#include <vector>
#include <cstdint>
#include <unordered_map>
#include <string>
#include "../common/Value.h" // Using the new high-level Value structure
#include "../common/Chunk.h" // The VM needs to know what a Chunk is!

class VM {
public:
    VM();
    
    // We pass a pointer to the Chunk // this is the main button for VM
    void interpret(Chunk* chunk); // i passed a pointer bcuz passing 10000lines of chunk directly firstly copy creation and then new memory allocation so this 
                                  // wastes memory as well as is too slow

private:
    // The Brains of the Operation 

    Chunk* currentChunk; // The chunk we are currently executing
    size_t ip;           // "Instruction Pointer": Points to the exact bytecode index we are running right now

    // The Memory 
    
    std::vector<Value> stack; // since we are doing stack-based implementation for VM
                              //now understand we are conceptually using the property of stack like pushing and popping etc
                              //but physically i am using  the vector because its fast and good in memory manangement as continuous blocks
                              // one more thing stack is  basically a wrapper like the plastic covering over the tv remote that is the vector or dequeue
    
    std::unordered_map<std::string, Value> globals; // Global Variables: A map to save variables permanently (like 'let x = 5')

    //  High-Performance Stack Operations 
    
    void push(Value value);          
    Value pop();                    
    Value peek(int distance = 0);  // Look at the stack without removing anything (0 is the top, 1 is just below the top)
};

#endif // VM_H
