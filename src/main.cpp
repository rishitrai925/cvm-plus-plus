#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <fstream>
#include <sstream>

void run(const std::string& source) {
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    std::cout << "[DEBUG] === Tokens ===\n";
    for (const Token& token : tokens) {
        std::cout << "[" << tokenTypeToString(token.type) << " | '" 
                  << token.lexeme << "' | Line " << token.line << "]\n";
    }

    try {
        Parser parser(tokens);
        std::vector<std::unique_ptr<ASTNode>> ast = parser.parseProgram();
        
        std::cout << "\n[DEBUG] === AST ===\n";
        for (const auto& stmt : ast) {
            stmt->print();
        }
    } catch (const ParseError& e) {
        std::cerr << "\n" << e.what() << "\n";
    }
}

void runFile(const char* path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        return;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    run(buffer.str());
}

void runREPL() {
    std::string line;
    std::cout << "CVM++ v1.0 — Interactive Mode\nType 'exit' to quit.\n";
    while (true) {
        std::cout << "\n>>> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "exit") {
            std::cout << "Bye!\n";
            break;
        }
        run(line);
    }
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        runREPL();
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        std::cerr << "Usage: cvm [script.cvm]" << std::endl;
        return 1;
    }
    return 0;
}
