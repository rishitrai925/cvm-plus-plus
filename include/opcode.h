#ifndef OPCODE_H
#define OPCODE_H

#include <cstdint>
#include <vector>
#include <string>

enum class OpCode : uint8_t {
    // Stack Operations
    OP_CONSTANT,     // Push a constant value onto the stack
    OP_POP,          // Pop top of stack (discard)

    // Arithmetic
    OP_ADD,          // Pop two, push sum
    OP_SUBTRACT,     // Pop two, push difference
    OP_MULTIPLY,     // Pop two, push product
    OP_DIVIDE,       // Pop two, push quotient
    OP_NEGATE,       // Pop one, push negated value

    // Comparison
    OP_EQUAL,        // Pop two, push bool (a == b)
    OP_NOT_EQUAL,    // Pop two, push bool (a != b)
    OP_LESS,         // Pop two, push bool (a < b)
    OP_GREATER,      // Pop two, push bool (a > b)
    OP_LESS_EQUAL,   // Pop two, push bool (a <= b)
    OP_GREATER_EQUAL,// Pop two, push bool (a >= b)

    // Logical
    OP_NOT,          // Pop one, push logical NOT

    // Boolean Literals
    OP_TRUE,         // Push true (1)
    OP_FALSE,        // Push false (0)

    // Variables
    OP_SET_GLOBAL,   // Pop value, store in variable (operand = name index)
    OP_GET_GLOBAL,   // Push variable value onto stack (operand = name index)

    // Control Flow
    OP_JUMP,         // Unconditional jump (operand = offset)
    OP_JUMP_IF_FALSE,// Pop condition; jump if false (operand = offset)

    // I/O
    OP_PRINT,        // Pop top of stack and print it
    OP_INPUT,        // Read integer from stdin, push onto stack

    // Program
    OP_HALT,         // Stop execution
};

struct Chunk {
    std::vector<uint8_t> code;        // The bytecode stream
    std::vector<int64_t> constants;   // Constant pool
    std::vector<std::string> names;   // Variable name pool

    int addConstant(int64_t value);   // Returns index
    int addName(const std::string& name); // Returns index
    void emit(OpCode op);
    void emit(OpCode op, uint8_t operand);
    void emit(OpCode op, uint16_t operand);  // For jumps (2-byte offset)
};

#endif // OPCODE_H
