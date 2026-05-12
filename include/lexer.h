#pragma once

#include "token.h"
#include <string>
#include <vector>
#include <unordered_map>

class Lexer {
public:
    Lexer(std::string source);
    std::vector<Token> tokenize();

private:
    std::string source;
    std::vector<Token> tokens;
    int start;
    int current;
    int line;

    std::unordered_map<std::string, TokenType> keywords;

    void initKeywords();
    bool isAtEnd() const;
    void scanToken();
    char advance();
    bool match(char expected);
    char peek() const;
    char peekNext() const;
    void skipWhitespace();
    
    void string(); // If strings are ever added, though we focus on numbers and idents for now
    void number();
    void identifier();

    void addToken(TokenType type);
    void addToken(TokenType type, const std::string& text);
    Token makeToken(TokenType type);
    Token errorToken(const std::string& message);
};
