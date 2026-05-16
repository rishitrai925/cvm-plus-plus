# CVM++ (Custom Virtual Machine in C++)

CVM++ is a lightweight, fully functional custom scripting language and stack-based Virtual Machine built entirely in C++. It features a complete pipeline from raw source code to bytecode execution.

## 🚀 Overview

CVM++ parses custom scripts (`.cvm` files) and translates them into an Abstract Syntax Tree (AST), which is then compiled into proprietary bytecode (opcodes) and executed sequentially by a stack-based runtime engine.

## 🏗 Architecture

The project consists of four primary modules:
1. **Lexer (Tokenizer)**: Scans raw source code and converts it into a stream of tokens.
2. **Parser (Recursive Descent)**: Constructs an Abstract Syntax Tree (AST) following defined BNF grammar.
3. **Compiler**: Translates the AST into bytecode chunks, handling variables, operator precedence, and backpatching for control flow offsets.
4. **Virtual Machine**: A stack-based runtime environment that executes opcodes linearly, managing global variables, execution stacks, and error handling.

## ✨ Features

- **Variables**: Dynamic variables declared with the `let` keyword.
- **Data Types**: Integers and Booleans (`true`, `false`).
- **Operators**: 
  - Arithmetic: `+`, `-`, `*`, `/`
  - Comparison: `<`, `>`, `<=`, `>=`, `==`, `!=`
  - Logical: `!` (NOT)
- **Control Flow**: 
  - `if / else` conditional branches.
  - `while` loops.
- **I/O**: Read from stdin with `input` and print to stdout with `print`.
- **REPL Interface**: Interactively run CVM++ scripts line-by-line.

## 🛠 Building the Project

Ensure you have a C++17 compliant compiler and CMake (3.15+) installed.

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## 💻 Usage

### 1. REPL Mode (Interactive)
Simply run the executable with no arguments:
```bash
./build/cvm
```
Inside the REPL, type `debug on` or `debug off` to toggle detailed compilation logs (tokens, AST, bytecode).

### 2. File Runner Mode
Pass a script file to execute it sequentially:
```bash
./build/cvm scripts/fibonacci.cvm
```

### 3. Debug Flags
You can inspect the compiler's internal pipeline at runtime using flags:
- `--tokens`: Prints the token stream from the Lexer.
- `--ast`: Prints the Abstract Syntax Tree.
- `--bytecode`: Prints the disassembled opcodes.
- `--debug`: Enables all the above flags.

Example:
```bash
./build/cvm --debug scripts/while_loop.cvm
```

## 📜 Example Script: Fibonacci Sequence

```
let a = 0;
let b = 1;
let n = 10;
let i = 0;

while (i < n) {
    print a;
    let temp = a + b;
    a = b;
    b = temp;
    i = i + 1;
}
```
*Output: `0 1 1 2 3 5 8 13 21 34`*

## 📂 Project Structure

- `include/`: Header files defining the core classes.
- `src/`: C++ implementations for the Lexer, Parser, Compiler, and VM.
- `scripts/`: Assorted `.cvm` test scripts demonstrating language features.
- `docs/`: Design notes, roadmap, and reference materials.
