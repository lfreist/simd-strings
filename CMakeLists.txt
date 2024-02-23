cmake_minimum_required(VERSION 3.23)
project(simd_string C CXX ASM)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(benchmark)

add_subdirectory(bindings/python)
add_subdirectory(bindings/cpp)

install(TARGETS simdstr_search utils
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)
install(DIRECTORY include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.h"
)