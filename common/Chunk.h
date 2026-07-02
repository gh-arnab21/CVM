#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <cstdint>
#include "Value.h"

// idea of chunk so that compiler return byte code alongwith another list of constants bcuz in byte code
// each single element is of a single byte so can;t store numbers out of 0-255 also can't deal with strings such as 'john'
struct Chunk {
    // The raw bytecode instructions
    std::vector<uint8_t> code;
    
    // The "Vault" of constants.
    
    std::vector<Value> constants;

    // Helper to add a byte to the chunk
    void writeByte(uint8_t byte) {
        code.push_back(byte);
    }

    // Helper to add a constant to the vault and return its index
    int addConstant(Value value) {
        constants.push_back(value);
        return constants.size() - 1; 
    }
};

#endif // CHUNK_H
