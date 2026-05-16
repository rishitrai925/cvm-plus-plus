#ifndef VM_H
#define VM_H

#include "opcode.h"
#include <vector>
#include <unordered_map>
#include <string>

class VM {
    Chunk chunk;
    size_t ip = 0;
    std::vector<int64_t> stack;
    std::unordered_map<std::string, int64_t> globals;

    uint8_t readByte();
    uint16_t readShort();
    int64_t pop();
    void push(int64_t v);
    int64_t peek(int distance = 0);
    void runtimeError(const std::string& msg);

public:
    void execute(const Chunk& bytecode);
};

#endif // VM_H
