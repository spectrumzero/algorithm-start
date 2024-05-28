cmake_minimum_required(VERSION 3.25.0)
project(Algorithm VERSION 1.0)

# specify the C standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

# 头文件所在目录
include_directories("./head/" "./utils/")

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(Algorithm "./chap4_tree/5_binarysearchtree.c")

