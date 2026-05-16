#include "lexer.h"
#include "parser.h"
#include "compiler.h"
#include "vm.h"
#include <iostream>
#include <fstream>
#include <sstream>

struct Options {
    bool showTokens = false;
    bool showAst = false;
    bool showBytecode = false;
};

void run(const std::string& source, VM& vm, const Options& options) {
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    if (options.showTokens) {
        std::cout << "[DEBUG] === Tokens ===\n";
        for (const Token& token : tokens) {
            std::cout << "[" << tokenTypeToString(token.type) << " | '" 
                      << token.lexeme << "' | Line " << token.line << "]\n";
        }
    }

    try {
        Parser parser(tokens);
        std::vector<std::unique_ptr<ASTNode>> ast = parser.parseProgram();
        
        if (options.showAst) {
            std::cout << "\n[DEBUG] === AST ===\n";
            for (const auto& stmt : ast) {
                stmt->print();
            }
        }

        Compiler compiler;
        Block rootBlock(std::move(ast));
        Chunk chunk = compiler.compile(&rootBlock);

        if (options.showBytecode) {
            std::cout << "\n[DEBUG] === Bytecode ===\n";
            Compiler::disassemble(chunk, "Main");
        }

        vm.execute(chunk);

    } catch (const std::exception& e) {
        std::cerr << "\nError: " << e.what() << "\n";
    }
}

void runFile(const char* path, const Options& options) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        return;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    
    VM vm;
    run(buffer.str(), vm, options);
}

void runREPL() {
    std::string line;
    std::cout << "CVM++ v1.0 — Interactive Mode\nType 'exit' to quit, 'debug on/off' to toggle debug mode.\n";
    
    VM vm;
    Options options;

    while (true) {
        std::cout << ">>> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "exit") {
            std::cout << "Bye!\n";
            break;
        }
        if (line == "debug on") {
            options.showTokens = true;
            options.showAst = true;
            options.showBytecode = true;
            std::cout << "Debug mode ON.\n";
            continue;
        }
        if (line == "debug off") {
            options.showTokens = false;
            options.showAst = false;
            options.showBytecode = false;
            std::cout << "Debug mode OFF.\n";
            continue;
        }
        
        if (line.empty()) continue;

        run(line, vm, options);
    }
}

int main(int argc, char* argv[]) {
    Options options;
    std::string filePath;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--tokens") options.showTokens = true;
        else if (arg == "--ast") options.showAst = true;
        else if (arg == "--bytecode") options.showBytecode = true;
        else if (arg == "--debug") {
            options.showTokens = true;
            options.showAst = true;
            options.showBytecode = true;
        }
        else if (filePath.empty()) {
            filePath = arg;
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            std::cerr << "Usage: cvm [options] [script.cvm]" << std::endl;
            return 1;
        }
    }

    if (filePath.empty()) {
        runREPL();
    } else {
        runFile(filePath.c_str(), options);
    }
    return 0;
}
