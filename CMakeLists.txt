cmake_minimum_required(VERSION 3.15)
project(CVMPlusPlus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES OFF)

add_executable(cvm
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/compiler.cpp
    src/vm.cpp
    src/opcode.cpp
)

target_include_directories(cvm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
