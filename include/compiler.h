#ifndef COMPILER_H
#define COMPILER_H

#include "ast.h"
#include "opcode.h"
#include <string>

class Compiler {
private:
    Chunk chunk;

    void compileNode(ASTNode* node);
    void compileNumberLiteral(NumberLiteral* node);
    void compileBoolLiteral(BoolLiteral* node);
    void compileIdentifier(Identifier* node);
    void compileBinaryExpr(BinaryExpr* node);
    void compileUnaryExpr(UnaryExpr* node);
    void compileAssignExpr(AssignExpr* node);
    
    void compileLetStmt(LetStmt* node);
    void compilePrintStmt(PrintStmt* node);
    void compileInputStmt(InputStmt* node);
    void compileIfStmt(IfStmt* node);
    void compileWhileStmt(WhileStmt* node);
    void compileExprStmt(ExprStmt* node);
    void compileBlock(Block* node);

    int emitJump(OpCode jumpOp);
    void patchJump(int offset);

public:
    Chunk compile(ASTNode* root);
    static void disassemble(const Chunk& chunk, const std::string& name);
};

#endif // COMPILER_H
