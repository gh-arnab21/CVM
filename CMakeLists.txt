cmake_minimum_required(VERSION 3.10)
project(CVM_PlusPlus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Professional compiler flags for a high-quality build
# -Wall and -Wextra enforce strict code quality
# -O3 optimizes the final Virtual Machine for maximum speed
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -O3")

# Add all source files dynamically
file(GLOB_RECURSE SOURCES 
    "lexer/*.cpp" 
    "parser/*.cpp" 
    "compiler/*.cpp" 
    "vm/*.cpp" 
    "common/*.cpp"
)

add_executable(cvm main.cpp ${SOURCES})
add_executable(cvm_lexer main_lexer.cpp ${SOURCES})
add_executable(cvm_parser main_parser.cpp ${SOURCES})
add_executable(cvm_compiler main_compiler.cpp ${SOURCES})
add_executable(cvm_vm main_vm.cpp ${SOURCES})
add_executable(cvm_final main_final.cpp ${SOURCES})
