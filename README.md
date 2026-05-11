# CVM++ — Custom Virtual Machine in C++

> A lightweight, custom scripting language with its own **Lexer**, **Parser**, **Bytecode Compiler**, and **Stack-Based Virtual Machine**, all built from scratch in C++17.

## 🏗️ Architecture

```
Source Code (.cvm)
       │
       ▼
   ┌────────┐
   │ Lexer  │  ──►  Tokens
   └────────┘
       │
       ▼
   ┌────────┐
   │ Parser │  ──►  Abstract Syntax Tree (AST)
   └────────┘
       │
       ▼
   ┌──────────┐
   │ Compiler │  ──►  Bytecode (Opcodes)
   └──────────┘
       │
       ▼
   ┌──────────────────┐
   │ Virtual Machine  │  ──►  Output
   │  (Stack-Based)   │
   └──────────────────┘
```

## 📦 Features

- **Custom scripting language** with C-like syntax
- **Lexer/Tokenizer** — converts source text to tokens
- **Recursive Descent Parser** — builds an Abstract Syntax Tree
- **Bytecode Compiler** — compiles AST to proprietary opcodes
- **Stack-Based VM** — executes bytecode with a 23-instruction ISA
- **REPL Mode** — interactive shell with persistent state
- **Debug Mode** — inspect tokens, AST, and bytecode at every stage

## 🚀 Building

### Prerequisites
- **g++** (GCC 10+ recommended) with C++17 support
- **CMake 3.15+** (optional)

### Build with CMake
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Build without CMake (direct g++)
```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/main.cpp src/lexer.cpp src/parser.cpp src/compiler.cpp src/vm.cpp -o cvm
```

## 📖 Usage

### Run a Script
```bash
./cvm scripts/fibonacci.cvm
```

### Interactive REPL
```bash
./cvm
CVM++ v1.0 — Interactive Mode
Type 'exit' to quit.

>>> let x = 42;
>>> print x;
42
>>> print x * 2 + 8;
92
>>> exit
Bye!
```

### Debug Flags
```bash
./cvm --tokens scripts/hello.cvm     # Show token list
./cvm --ast scripts/hello.cvm        # Show AST tree
./cvm --bytecode scripts/hello.cvm   # Show bytecode disassembly
./cvm --debug scripts/hello.cvm      # Show everything
```

## 📝 Language Reference

### Data Types
| Type | Examples | Internal |
|------|----------|----------|
| Integer | `0`, `42`, `-7` | `int64_t` |
| Boolean | `true`, `false` | `int64_t` (1/0) |

### Variables
```
let x = 10;
let y = x + 20;
x = 42;           // reassignment
```

### Arithmetic
```
print 10 + 20;     // 30
print 100 - 25;    // 75
print 6 * 7;       // 42
print 100 / 4;     // 25
print -42;         // -42
```

### Comparisons & Boolean Logic
```
print 10 == 10;    // 1 (true)
print 10 != 20;    // 1 (true)
print 5 < 10;      // 1 (true)
print 5 > 10;      // 0 (false)
print !true;       // 0 (false)
```

### Control Flow
```
if (x > 10) {
    print 1;
} else {
    print 0;
}

while (i < 10) {
    print i;
    i = i + 1;
}
```

### I/O
```
print 42;          // Output: 42
input x;           // Read integer from stdin into x
```

### Comments
```
// This is a line comment
let x = 10; // inline comment
```

## 📂 Project Structure

```
cvm++/
├── CMakeLists.txt              # Build configuration
├── README.md                   # This file
├── include/
│   ├── token.h                 # Token types & Token struct
│   ├── value.h                 # Runtime value type
│   ├── lexer.h                 # Lexer declarations
│   ├── ast.h                   # AST node definitions
│   ├── parser.h                # Parser declarations
│   ├── opcode.h                # Opcode enum & Chunk struct
│   ├── compiler.h              # Compiler declarations
│   └── vm.h                    # VM declarations
├── src/
│   ├── main.cpp                # Entry point (REPL + file runner)
│   ├── lexer.cpp               # Lexer implementation
│   ├── parser.cpp              # Parser implementation
│   ├── compiler.cpp            # Compiler implementation
│   └── vm.cpp                  # VM implementation
└── scripts/
    ├── hello.cvm               # Basic demo
    ├── arithmetic.cvm          # Math operations
    ├── variables.cvm           # Variable usage
    ├── booleans.cvm            # Boolean logic
    ├── control_flow.cvm        # If/else & while
    ├── fibonacci.cvm           # Fibonacci sequence
    └── input_output.cvm        # I/O demo
```

## 📚 Inspired By
- [Crafting Interpreters](https://craftinginterpreters.com/) by Robert Nystrom
