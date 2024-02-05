cmake_minimum_required(VERSION 3.23)
project(simd_string C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(benchmark)