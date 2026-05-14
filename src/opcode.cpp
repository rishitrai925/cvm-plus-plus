#include "opcode.h"

int Chunk::addConstant(int64_t value) {
    constants.push_back(value);
    return constants.size() - 1;
}

int Chunk::addName(const std::string& name) {
    for (size_t i = 0; i < names.size(); ++i) {
        if (names[i] == name) {
            return i;
        }
    }
    names.push_back(name);
    return names.size() - 1;
}

void Chunk::emit(OpCode op) {
    code.push_back(static_cast<uint8_t>(op));
}

void Chunk::emit(OpCode op, uint8_t operand) {
    code.push_back(static_cast<uint8_t>(op));
    code.push_back(operand);
}

void Chunk::emit(OpCode op, uint16_t operand) {
    code.push_back(static_cast<uint8_t>(op));
    code.push_back(static_cast<uint8_t>((operand >> 8) & 0xFF));
    code.push_back(static_cast<uint8_t>(operand & 0xFF));
}
