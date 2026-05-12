#pragma once

#include "token.h"
#include "ast.h"
#include <vector>
#include <stdexcept>
#include <memory>

class ParseError : public std::runtime_error {
public:
    explicit ParseError(const std::string& message) : std::runtime_error(message) {}
};

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    
    // Parse the entire program (list of statements)
    std::vector<std::unique_ptr<ASTNode>> parseProgram();

private:
    const std::vector<Token>& tokens;
    int current = 0;

    // --- Grammar rules ---
    std::unique_ptr<ASTNode> parseStatement();
    std::unique_ptr<ASTNode> parseLetStmt();
    std::unique_ptr<ASTNode> parsePrintStmt();
    std::unique_ptr<ASTNode> parseInputStmt();
    std::unique_ptr<ASTNode> parseIfStmt();
    std::unique_ptr<ASTNode> parseWhileStmt();
    std::unique_ptr<ASTNode> parseExprStmt();
    std::unique_ptr<Block> parseBlock();

    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parseAssignment();
    std::unique_ptr<ASTNode> parseEquality();
    std::unique_ptr<ASTNode> parseComparison();
    std::unique_ptr<ASTNode> parseTerm();
    std::unique_ptr<ASTNode> parseFactor();
    std::unique_ptr<ASTNode> parseUnary();
    std::unique_ptr<ASTNode> parsePrimary();

    // --- Helper methods ---
    bool isAtEnd();
    Token peek();
    Token previous();
    bool check(TokenType type);
    Token advance();
    bool match(TokenType type);
    Token consume(TokenType type, const std::string& message);
    void error(Token token, const std::string& message);
};
