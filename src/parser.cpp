#include "parser.h"
#include <iostream>

// --- AST Print Implementations ---

void printIndent(int indent) {
    for (int i = 0; i < indent; ++i) std::cout << "  ";
}

void NumberLiteral::print(int indent) const {
    printIndent(indent);
    std::cout << "NumberLiteral(" << value << ")\n";
}

void BoolLiteral::print(int indent) const {
    printIndent(indent);
    std::cout << "BoolLiteral(" << (value ? "true" : "false") << ")\n";
}

void Identifier::print(int indent) const {
    printIndent(indent);
    std::cout << "Identifier(" << name << ")\n";
}

void UnaryExpr::print(int indent) const {
    printIndent(indent);
    std::cout << "UnaryExpr(" << tokenTypeToString(op) << ")\n";
    operand->print(indent + 1);
}

void BinaryExpr::print(int indent) const {
    printIndent(indent);
    std::cout << "BinaryExpr(" << tokenTypeToString(op) << ")\n";
    left->print(indent + 1);
    right->print(indent + 1);
}

void AssignExpr::print(int indent) const {
    printIndent(indent);
    std::cout << "AssignExpr(" << name << ")\n";
    value->print(indent + 1);
}

void ExprStmt::print(int indent) const {
    printIndent(indent);
    std::cout << "ExprStmt\n";
    expression->print(indent + 1);
}

void LetStmt::print(int indent) const {
    printIndent(indent);
    std::cout << "LetStmt(" << name << ")\n";
    initializer->print(indent + 1);
}

void PrintStmt::print(int indent) const {
    printIndent(indent);
    std::cout << "PrintStmt\n";
    expression->print(indent + 1);
}

void InputStmt::print(int indent) const {
    printIndent(indent);
    std::cout << "InputStmt(" << variableName << ")\n";
}

void Block::print(int indent) const {
    printIndent(indent);
    std::cout << "Block\n";
    for (const auto& stmt : statements) {
        stmt->print(indent + 1);
    }
}

void IfStmt::print(int indent) const {
    printIndent(indent);
    std::cout << "IfStmt\n";
    printIndent(indent + 1); std::cout << "Condition:\n";
    condition->print(indent + 2);
    printIndent(indent + 1); std::cout << "Then:\n";
    thenBranch->print(indent + 2);
    if (elseBranch) {
        printIndent(indent + 1); std::cout << "Else:\n";
        elseBranch->print(indent + 2);
    }
}

void WhileStmt::print(int indent) const {
    printIndent(indent);
    std::cout << "WhileStmt\n";
    printIndent(indent + 1); std::cout << "Condition:\n";
    condition->print(indent + 2);
    printIndent(indent + 1); std::cout << "Body:\n";
    body->print(indent + 2);
}

// --- Parser Implementation ---

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

std::vector<std::unique_ptr<ASTNode>> Parser::parseProgram() {
    std::vector<std::unique_ptr<ASTNode>> statements;
    while (!isAtEnd()) {
        statements.push_back(parseStatement());
    }
    return statements;
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    if (match(TokenType::LET)) return parseLetStmt();
    if (match(TokenType::PRINT)) return parsePrintStmt();
    if (match(TokenType::INPUT)) return parseInputStmt();
    if (match(TokenType::IF)) return parseIfStmt();
    if (match(TokenType::WHILE)) return parseWhileStmt();
    return parseExprStmt();
}

std::unique_ptr<ASTNode> Parser::parseLetStmt() {
    Token nameToken = consume(TokenType::IDENTIFIER, "Expect variable name.");
    consume(TokenType::EQUAL, "Expect '=' after variable name.");
    std::unique_ptr<ASTNode> initializer = parseExpression();
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_unique<LetStmt>(nameToken.lexeme, std::move(initializer));
}

std::unique_ptr<ASTNode> Parser::parsePrintStmt() {
    std::unique_ptr<ASTNode> value = parseExpression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_unique<PrintStmt>(std::move(value));
}

std::unique_ptr<ASTNode> Parser::parseInputStmt() {
    Token nameToken = consume(TokenType::IDENTIFIER, "Expect variable name for input.");
    consume(TokenType::SEMICOLON, "Expect ';' after input statement.");
    return std::make_unique<InputStmt>(nameToken.lexeme);
}

std::unique_ptr<ASTNode> Parser::parseIfStmt() {
    consume(TokenType::LPAREN, "Expect '(' after 'if'.");
    std::unique_ptr<ASTNode> condition = parseExpression();
    consume(TokenType::RPAREN, "Expect ')' after if condition.");
    
    std::unique_ptr<Block> thenBranch = parseBlock();
    std::unique_ptr<Block> elseBranch = nullptr;
    
    if (match(TokenType::ELSE)) {
        elseBranch = parseBlock();
    }
    
    return std::make_unique<IfStmt>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

std::unique_ptr<ASTNode> Parser::parseWhileStmt() {
    consume(TokenType::LPAREN, "Expect '(' after 'while'.");
    std::unique_ptr<ASTNode> condition = parseExpression();
    consume(TokenType::RPAREN, "Expect ')' after while condition.");
    
    std::unique_ptr<Block> body = parseBlock();
    return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
}

std::unique_ptr<Block> Parser::parseBlock() {
    consume(TokenType::LBRACE, "Expect '{' before block.");
    std::vector<std::unique_ptr<ASTNode>> statements;
    while (!check(TokenType::RBRACE) && !isAtEnd()) {
        statements.push_back(parseStatement());
    }
    consume(TokenType::RBRACE, "Expect '}' after block.");
    return std::make_unique<Block>(std::move(statements));
}

std::unique_ptr<ASTNode> Parser::parseExprStmt() {
    std::unique_ptr<ASTNode> expr = parseExpression();
    consume(TokenType::SEMICOLON, "Expect ';' after expression.");
    return std::make_unique<ExprStmt>(std::move(expr));
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
    return parseAssignment();
}

std::unique_ptr<ASTNode> Parser::parseAssignment() {
    std::unique_ptr<ASTNode> expr = parseEquality();

    if (match(TokenType::EQUAL)) {
        Token equals = previous();
        std::unique_ptr<ASTNode> value = parseAssignment();

        if (Identifier* ident = dynamic_cast<Identifier*>(expr.get())) {
            return std::make_unique<AssignExpr>(ident->name, std::move(value));
        }

        error(equals, "Invalid assignment target.");
    }

    return expr;
}

std::unique_ptr<ASTNode> Parser::parseEquality() {
    std::unique_ptr<ASTNode> expr = parseComparison();

    while (match(TokenType::BANG_EQUAL) || match(TokenType::EQUAL_EQUAL)) {
        TokenType op = previous().type;
        std::unique_ptr<ASTNode> right = parseComparison();
        expr = std::make_unique<BinaryExpr>(op, std::move(expr), std::move(right));
    }

    return expr;
}

std::unique_ptr<ASTNode> Parser::parseComparison() {
    std::unique_ptr<ASTNode> expr = parseTerm();

    while (match(TokenType::GREATER) || match(TokenType::GREATER_EQUAL) || 
           match(TokenType::LESS) || match(TokenType::LESS_EQUAL)) {
        TokenType op = previous().type;
        std::unique_ptr<ASTNode> right = parseTerm();
        expr = std::make_unique<BinaryExpr>(op, std::move(expr), std::move(right));
    }

    return expr;
}

std::unique_ptr<ASTNode> Parser::parseTerm() {
    std::unique_ptr<ASTNode> expr = parseFactor();

    while (match(TokenType::MINUS) || match(TokenType::PLUS)) {
        TokenType op = previous().type;
        std::unique_ptr<ASTNode> right = parseFactor();
        expr = std::make_unique<BinaryExpr>(op, std::move(expr), std::move(right));
    }

    return expr;
}

std::unique_ptr<ASTNode> Parser::parseFactor() {
    std::unique_ptr<ASTNode> expr = parseUnary();

    while (match(TokenType::SLASH) || match(TokenType::STAR)) {
        TokenType op = previous().type;
        std::unique_ptr<ASTNode> right = parseUnary();
        expr = std::make_unique<BinaryExpr>(op, std::move(expr), std::move(right));
    }

    return expr;
}

std::unique_ptr<ASTNode> Parser::parseUnary() {
    if (match(TokenType::BANG) || match(TokenType::MINUS)) {
        TokenType op = previous().type;
        std::unique_ptr<ASTNode> right = parseUnary();
        return std::make_unique<UnaryExpr>(op, std::move(right));
    }

    return parsePrimary();
}

std::unique_ptr<ASTNode> Parser::parsePrimary() {
    if (match(TokenType::FALSE_KW)) return std::make_unique<BoolLiteral>(false);
    if (match(TokenType::TRUE_KW)) return std::make_unique<BoolLiteral>(true);

    if (match(TokenType::NUMBER)) {
        return std::make_unique<NumberLiteral>(std::stoll(previous().lexeme));
    }

    if (match(TokenType::IDENTIFIER)) {
        return std::make_unique<Identifier>(previous().lexeme);
    }

    if (match(TokenType::LPAREN)) {
        std::unique_ptr<ASTNode> expr = parseExpression();
        consume(TokenType::RPAREN, "Expect ')' after expression.");
        return expr;
    }

    error(peek(), "Expect expression.");
    return nullptr;
}

// --- Helper Methods ---

bool Parser::isAtEnd() {
    return peek().type == TokenType::EOF_TOKEN;
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    error(peek(), message);
    return peek(); // Never reached due to exception
}

void Parser::error(Token token, const std::string& message) {
    std::string err = "Parse Error at ";
    if (token.type == TokenType::EOF_TOKEN) {
        err += "end";
    } else {
        err += "'" + token.lexeme + "' (line " + std::to_string(token.line) + ")";
    }
    err += ": " + message;
    throw ParseError(err);
}
