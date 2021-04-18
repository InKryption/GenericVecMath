cmake_minimum_required(VERSION 3.1)

project(VecMath VERSION 1.0)

add_executable(TEST)
target_compile_options(TEST PUBLIC -std=c++2a)
add_subdirectory(src)
