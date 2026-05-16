#include "vm.h"
#include <iostream>
#include <stdexcept>

uint8_t VM::readByte() {
    return chunk.code[ip++];
}

uint16_t VM::readShort() {
    uint16_t high = chunk.code[ip++];
    uint16_t low = chunk.code[ip++];
    return (high << 8) | low;
}

int64_t VM::pop() {
    if (stack.empty()) {
        runtimeError("Stack underflow.");
    }
    int64_t v = stack.back();
    stack.pop_back();
    return v;
}

void VM::push(int64_t v) {
    stack.push_back(v);
}

int64_t VM::peek(int distance) {
    if (stack.size() <= distance) {
        runtimeError("Stack underflow on peek.");
    }
    return stack[stack.size() - 1 - distance];
}

void VM::runtimeError(const std::string& msg) {
    std::cerr << "Runtime Error: " << msg << std::endl;
    std::cerr << "Instruction Pointer: " << ip << std::endl;
    throw std::runtime_error(msg);
}

void VM::execute(const Chunk& bytecode) {
    this->chunk = bytecode;
    this->ip = 0;
    this->stack.clear();

    while (ip < chunk.code.size()) {
        OpCode instruction = static_cast<OpCode>(readByte());

        switch (instruction) {
            case OpCode::OP_CONSTANT: {
                uint8_t index = readByte();
                push(chunk.constants[index]);
                break;
            }
            case OpCode::OP_POP: {
                pop();
                break;
            }
            case OpCode::OP_ADD: {
                int64_t b = pop();
                int64_t a = pop();
                push(a + b);
                break;
            }
            case OpCode::OP_SUBTRACT: {
                int64_t b = pop();
                int64_t a = pop();
                push(a - b);
                break;
            }
            case OpCode::OP_MULTIPLY: {
                int64_t b = pop();
                int64_t a = pop();
                push(a * b);
                break;
            }
            case OpCode::OP_DIVIDE: {
                int64_t b = pop();
                int64_t a = pop();
                if (b == 0) {
                    runtimeError("Division by zero");
                }
                push(a / b);
                break;
            }
            case OpCode::OP_NEGATE: {
                push(-pop());
                break;
            }
            case OpCode::OP_EQUAL: {
                int64_t b = pop();
                int64_t a = pop();
                push(a == b ? 1 : 0);
                break;
            }
            case OpCode::OP_NOT_EQUAL: {
                int64_t b = pop();
                int64_t a = pop();
                push(a != b ? 1 : 0);
                break;
            }
            case OpCode::OP_LESS: {
                int64_t b = pop();
                int64_t a = pop();
                push(a < b ? 1 : 0);
                break;
            }
            case OpCode::OP_GREATER: {
                int64_t b = pop();
                int64_t a = pop();
                push(a > b ? 1 : 0);
                break;
            }
            case OpCode::OP_LESS_EQUAL: {
                int64_t b = pop();
                int64_t a = pop();
                push(a <= b ? 1 : 0);
                break;
            }
            case OpCode::OP_GREATER_EQUAL: {
                int64_t b = pop();
                int64_t a = pop();
                push(a >= b ? 1 : 0);
                break;
            }
            case OpCode::OP_NOT: {
                push(pop() == 0 ? 1 : 0);
                break;
            }
            case OpCode::OP_TRUE: {
                push(1);
                break;
            }
            case OpCode::OP_FALSE: {
                push(0);
                break;
            }
            case OpCode::OP_SET_GLOBAL: {
                uint8_t nameIdx = readByte();
                globals[chunk.names[nameIdx]] = pop();
                break;
            }
            case OpCode::OP_GET_GLOBAL: {
                uint8_t nameIdx = readByte();
                auto it = globals.find(chunk.names[nameIdx]);
                if (it == globals.end()) {
                    runtimeError("Undefined variable: " + chunk.names[nameIdx]);
                }
                push(it->second);
                break;
            }
            case OpCode::OP_JUMP: {
                uint16_t offset = readShort();
                ip += static_cast<int16_t>(offset);
                break;
            }
            case OpCode::OP_JUMP_IF_FALSE: {
                uint16_t offset = readShort();
                if (pop() == 0) {
                    ip += static_cast<int16_t>(offset);
                }
                break;
            }
            case OpCode::OP_PRINT: {
                std::cout << pop() << std::endl;
                break;
            }
            case OpCode::OP_INPUT: {
                int64_t val;
                std::cin >> val;
                push(val);
                break;
            }
            case OpCode::OP_HALT: {
                return;
            }
            default: {
                runtimeError("Unknown opcode");
            }
        }
    }
}
