#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <variant>

// ---------------------------------------------------------
// TODO: Professional Value Representation
// Instead of using raw 'int' for the VM stack, a high-level 
// project uses a tagged union or std::variant to support 
// multiple data types (Integers, Booleans, etc.) cleanly.
// ---------------------------------------------------------

enum class ValueType {
    VAL_BOOL,
    VAL_INT,
    VAL_STRING // Added to support variable names!
};

// Represents any value inside the Virtual Machine
struct Value {
    ValueType type;
    std::variant<bool, int, std::string> as;
    
    // Helper methods for safe type checking
    bool isBool() const { return type == ValueType::VAL_BOOL; }
    bool isInt() const { return type == ValueType::VAL_INT; }
    bool isString() const { return type == ValueType::VAL_STRING; }
};

// ---------------------------------------------------------
// TODO: Add a helper function in a .cpp file to print values
// void printValue(Value value);
// ---------------------------------------------------------

#endif // VALUE_H
