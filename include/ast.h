#pragma once

#include <memory>
#include <string>
#include <vector>
#include <cstdint>
#include "token.h"

// Base AST Node
struct ASTNode {
    virtual ~ASTNode() = default;
    
    // Abstract method for AST printing (Visitor pattern could be better, but this is simpler for now)
    virtual void print(int indent = 0) const = 0;
};

// --- Expressions ---

struct NumberLiteral : public ASTNode {
    int64_t value;
    
    explicit NumberLiteral(int64_t val) : value(val) {}
    void print(int indent = 0) const override;
};

struct BoolLiteral : public ASTNode {
    bool value;
    
    explicit BoolLiteral(bool val) : value(val) {}
    void print(int indent = 0) const override;
};

struct Identifier : public ASTNode {
    std::string name;
    
    explicit Identifier(std::string n) : name(std::move(n)) {}
    void print(int indent = 0) const override;
};

struct UnaryExpr : public ASTNode {
    TokenType op;
    std::unique_ptr<ASTNode> operand;
    
    UnaryExpr(TokenType op, std::unique_ptr<ASTNode> operand)
        : op(op), operand(std::move(operand)) {}
    void print(int indent = 0) const override;
};

struct BinaryExpr : public ASTNode {
    TokenType op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
    
    BinaryExpr(TokenType op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
        : op(op), left(std::move(left)), right(std::move(right)) {}
    void print(int indent = 0) const override;
};

struct AssignExpr : public ASTNode {
    std::string name;
    std::unique_ptr<ASTNode> value;
    
    AssignExpr(std::string name, std::unique_ptr<ASTNode> value)
        : name(std::move(name)), value(std::move(value)) {}
    void print(int indent = 0) const override;
};

// --- Statements ---

struct ExprStmt : public ASTNode {
    std::unique_ptr<ASTNode> expression;
    
    explicit ExprStmt(std::unique_ptr<ASTNode> expr) : expression(std::move(expr)) {}
    void print(int indent = 0) const override;
};

struct LetStmt : public ASTNode {
    std::string name;
    std::unique_ptr<ASTNode> initializer;
    
    LetStmt(std::string name, std::unique_ptr<ASTNode> init)
        : name(std::move(name)), initializer(std::move(init)) {}
    void print(int indent = 0) const override;
};

struct PrintStmt : public ASTNode {
    std::unique_ptr<ASTNode> expression;
    
    explicit PrintStmt(std::unique_ptr<ASTNode> expr) : expression(std::move(expr)) {}
    void print(int indent = 0) const override;
};

struct InputStmt : public ASTNode {
    std::string variableName;
    
    explicit InputStmt(std::string name) : variableName(std::move(name)) {}
    void print(int indent = 0) const override;
};

struct Block : public ASTNode {
    std::vector<std::unique_ptr<ASTNode>> statements;
    
    explicit Block(std::vector<std::unique_ptr<ASTNode>> stmts) : statements(std::move(stmts)) {}
    void print(int indent = 0) const override;
};

struct IfStmt : public ASTNode {
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<Block> thenBranch;
    std::unique_ptr<Block> elseBranch; // nullptr if no else
    
    IfStmt(std::unique_ptr<ASTNode> cond, std::unique_ptr<Block> thenB, std::unique_ptr<Block> elseB)
        : condition(std::move(cond)), thenBranch(std::move(thenB)), elseBranch(std::move(elseB)) {}
    void print(int indent = 0) const override;
};

struct WhileStmt : public ASTNode {
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<Block> body;
    
    WhileStmt(std::unique_ptr<ASTNode> cond, std::unique_ptr<Block> body)
        : condition(std::move(cond)), body(std::move(body)) {}
    void print(int indent = 0) const override;
};
