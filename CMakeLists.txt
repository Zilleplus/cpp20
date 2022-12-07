cmake_minimum_required(VERSION 3.23)
project(cpp20)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(/W4)

add_subdirectory(coroutines)