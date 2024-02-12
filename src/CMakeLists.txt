add_subdirectory(utils)

add_library(simdstr_search search.c)
target_link_libraries(simdstr_search PUBLIC utils)
target_compile_options(simdstr_search PUBLIC "-mavx2")