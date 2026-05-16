#include "compiler.h"
#include <iostream>
#include <iomanip>

Chunk Compiler::compile(ASTNode* root) {
    chunk = Chunk();
    if (root) {
        compileNode(root);
        chunk.emit(OpCode::OP_HALT);
    }
    return chunk;
}

void Compiler::compileNode(ASTNode* node) {
    if (!node) return;

    if (auto n = dynamic_cast<NumberLiteral*>(node)) { compileNumberLiteral(n); }
    else if (auto b = dynamic_cast<BoolLiteral*>(node)) { compileBoolLiteral(b); }
    else if (auto id = dynamic_cast<Identifier*>(node)) { compileIdentifier(id); }
    else if (auto bin = dynamic_cast<BinaryExpr*>(node)) { compileBinaryExpr(bin); }
    else if (auto un = dynamic_cast<UnaryExpr*>(node)) { compileUnaryExpr(un); }
    else if (auto asgn = dynamic_cast<AssignExpr*>(node)) { compileAssignExpr(asgn); }
    else if (auto let = dynamic_cast<LetStmt*>(node)) { compileLetStmt(let); }
    else if (auto prt = dynamic_cast<PrintStmt*>(node)) { compilePrintStmt(prt); }
    else if (auto inp = dynamic_cast<InputStmt*>(node)) { compileInputStmt(inp); }
    else if (auto ifs = dynamic_cast<IfStmt*>(node)) { compileIfStmt(ifs); }
    else if (auto whs = dynamic_cast<WhileStmt*>(node)) { compileWhileStmt(whs); }
    else if (auto exprs = dynamic_cast<ExprStmt*>(node)) { compileExprStmt(exprs); }
    else if (auto blk = dynamic_cast<Block*>(node)) { compileBlock(blk); }
}

void Compiler::compileNumberLiteral(NumberLiteral* node) {
    int idx = chunk.addConstant(node->value);
    chunk.emit(OpCode::OP_CONSTANT, static_cast<uint8_t>(idx));
}

void Compiler::compileBoolLiteral(BoolLiteral* node) {
    if (node->value) {
        chunk.emit(OpCode::OP_TRUE);
    } else {
        chunk.emit(OpCode::OP_FALSE);
    }
}

void Compiler::compileIdentifier(Identifier* node) {
    int idx = chunk.addName(node->name);
    chunk.emit(OpCode::OP_GET_GLOBAL, static_cast<uint8_t>(idx));
}

void Compiler::compileBinaryExpr(BinaryExpr* node) {
    compileNode(node->left.get());
    compileNode(node->right.get());
    switch (node->op) {
        case TokenType::PLUS:          chunk.emit(OpCode::OP_ADD); break;
        case TokenType::MINUS:         chunk.emit(OpCode::OP_SUBTRACT); break;
        case TokenType::STAR:          chunk.emit(OpCode::OP_MULTIPLY); break;
        case TokenType::SLASH:         chunk.emit(OpCode::OP_DIVIDE); break;
        case TokenType::EQUAL_EQUAL:   chunk.emit(OpCode::OP_EQUAL); break;
        case TokenType::BANG_EQUAL:    chunk.emit(OpCode::OP_NOT_EQUAL); break;
        case TokenType::LESS:          chunk.emit(OpCode::OP_LESS); break;
        case TokenType::GREATER:       chunk.emit(OpCode::OP_GREATER); break;
        case TokenType::LESS_EQUAL:    chunk.emit(OpCode::OP_LESS_EQUAL); break;
        case TokenType::GREATER_EQUAL: chunk.emit(OpCode::OP_GREATER_EQUAL); break;
        default: break;
    }
}

void Compiler::compileUnaryExpr(UnaryExpr* node) {
    compileNode(node->operand.get());
    switch (node->op) {
        case TokenType::MINUS: chunk.emit(OpCode::OP_NEGATE); break;
        case TokenType::BANG:  chunk.emit(OpCode::OP_NOT); break;
        default: break;
    }
}

void Compiler::compileAssignExpr(AssignExpr* node) {
    compileNode(node->value.get());
    int idx = chunk.addName(node->name);
    chunk.emit(OpCode::OP_SET_GLOBAL, static_cast<uint8_t>(idx));
    chunk.emit(OpCode::OP_GET_GLOBAL, static_cast<uint8_t>(idx));
}

void Compiler::compileLetStmt(LetStmt* node) {
    if (node->initializer) {
        compileNode(node->initializer.get());
    } else {
        chunk.emit(OpCode::OP_FALSE); // default init
    }
    int idx = chunk.addName(node->name);
    chunk.emit(OpCode::OP_SET_GLOBAL, static_cast<uint8_t>(idx));
}

void Compiler::compilePrintStmt(PrintStmt* node) {
    compileNode(node->expression.get());
    chunk.emit(OpCode::OP_PRINT);
}

void Compiler::compileInputStmt(InputStmt* node) {
    chunk.emit(OpCode::OP_INPUT);
    int idx = chunk.addName(node->variableName);
    chunk.emit(OpCode::OP_SET_GLOBAL, static_cast<uint8_t>(idx));
}

void Compiler::compileIfStmt(IfStmt* node) {
    compileNode(node->condition.get());
    int elseJump = emitJump(OpCode::OP_JUMP_IF_FALSE);
    
    if (node->thenBranch) {
        compileNode(node->thenBranch.get());
    }

    int endJump = emitJump(OpCode::OP_JUMP);

    patchJump(elseJump);

    if (node->elseBranch) {
        compileNode(node->elseBranch.get());
    }

    patchJump(endJump);
}

void Compiler::compileWhileStmt(WhileStmt* node) {
    int loopStart = chunk.code.size();
    
    compileNode(node->condition.get());
    int exitJump = emitJump(OpCode::OP_JUMP_IF_FALSE);

    if (node->body) {
        compileNode(node->body.get());
    }

    int jumpToStart = loopStart - (chunk.code.size() + 3);
    chunk.emit(OpCode::OP_JUMP);
    chunk.code.push_back(static_cast<uint8_t>((jumpToStart >> 8) & 0xFF));
    chunk.code.push_back(static_cast<uint8_t>(jumpToStart & 0xFF));

    patchJump(exitJump);
}

void Compiler::compileExprStmt(ExprStmt* node) {
    compileNode(node->expression.get());
    chunk.emit(OpCode::OP_POP);
}

void Compiler::compileBlock(Block* node) {
    for (auto& stmt : node->statements) {
        compileNode(stmt.get());
    }
}

int Compiler::emitJump(OpCode jumpOp) {
    chunk.emit(jumpOp);
    chunk.code.push_back(static_cast<uint8_t>(0xFF));
    chunk.code.push_back(static_cast<uint8_t>(0xFF));
    return chunk.code.size() - 2;
}

void Compiler::patchJump(int offset) {
    int jump = chunk.code.size() - offset - 2;
    chunk.code[offset]     = (jump >> 8) & 0xFF;
    chunk.code[offset + 1] = jump & 0xFF;
}

void Compiler::disassemble(const Chunk& chunk, const std::string& name) {
    std::cout << "=== " << name << " ===" << std::endl;
    for (size_t offset = 0; offset < chunk.code.size(); ) {
        std::cout << std::setfill('0') << std::setw(4) << offset << "  ";
        OpCode instruction = static_cast<OpCode>(chunk.code[offset]);

        switch (instruction) {
            case OpCode::OP_CONSTANT: {
                uint8_t constant = chunk.code[offset + 1];
                std::cout << "OP_CONSTANT      " << (int)constant 
                          << "    (" << chunk.constants[constant] << ")" << std::endl;
                offset += 2;
                break;
            }
            case OpCode::OP_POP: std::cout << "OP_POP" << std::endl; offset++; break;
            case OpCode::OP_ADD: std::cout << "OP_ADD" << std::endl; offset++; break;
            case OpCode::OP_SUBTRACT: std::cout << "OP_SUBTRACT" << std::endl; offset++; break;
            case OpCode::OP_MULTIPLY: std::cout << "OP_MULTIPLY" << std::endl; offset++; break;
            case OpCode::OP_DIVIDE: std::cout << "OP_DIVIDE" << std::endl; offset++; break;
            case OpCode::OP_NEGATE: std::cout << "OP_NEGATE" << std::endl; offset++; break;
            case OpCode::OP_EQUAL: std::cout << "OP_EQUAL" << std::endl; offset++; break;
            case OpCode::OP_NOT_EQUAL: std::cout << "OP_NOT_EQUAL" << std::endl; offset++; break;
            case OpCode::OP_LESS: std::cout << "OP_LESS" << std::endl; offset++; break;
            case OpCode::OP_GREATER: std::cout << "OP_GREATER" << std::endl; offset++; break;
            case OpCode::OP_LESS_EQUAL: std::cout << "OP_LESS_EQUAL" << std::endl; offset++; break;
            case OpCode::OP_GREATER_EQUAL: std::cout << "OP_GREATER_EQUAL" << std::endl; offset++; break;
            case OpCode::OP_NOT: std::cout << "OP_NOT" << std::endl; offset++; break;
            case OpCode::OP_TRUE: std::cout << "OP_TRUE" << std::endl; offset++; break;
            case OpCode::OP_FALSE: std::cout << "OP_FALSE" << std::endl; offset++; break;
            case OpCode::OP_SET_GLOBAL: {
                uint8_t nameIdx = chunk.code[offset + 1];
                std::cout << "OP_SET_GLOBAL    " << (int)nameIdx 
                          << "    (" << chunk.names[nameIdx] << ")" << std::endl;
                offset += 2;
                break;
            }
            case OpCode::OP_GET_GLOBAL: {
                uint8_t nameIdx = chunk.code[offset + 1];
                std::cout << "OP_GET_GLOBAL    " << (int)nameIdx 
                          << "    (" << chunk.names[nameIdx] << ")" << std::endl;
                offset += 2;
                break;
            }
            case OpCode::OP_JUMP: {
                uint16_t jump = (chunk.code[offset + 1] << 8) | chunk.code[offset + 2];
                std::cout << "OP_JUMP          " << jump << std::endl;
                offset += 3;
                break;
            }
            case OpCode::OP_JUMP_IF_FALSE: {
                uint16_t jump = (chunk.code[offset + 1] << 8) | chunk.code[offset + 2];
                std::cout << "OP_JUMP_IF_FALSE " << jump << std::endl;
                offset += 3;
                break;
            }
            case OpCode::OP_PRINT: std::cout << "OP_PRINT" << std::endl; offset++; break;
            case OpCode::OP_INPUT: std::cout << "OP_INPUT" << std::endl; offset++; break;
            case OpCode::OP_HALT: std::cout << "OP_HALT" << std::endl; offset++; break;
            default:
                std::cout << "Unknown opcode " << (int)instruction << std::endl;
                offset++;
                break;
        }
    }
}
