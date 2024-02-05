add_library(simdstr_search search.c)
target_compile_options(simdstr_search PUBLIC "-mavx2")

add_library(simdstr STATIC simdstr.c)
target_link_libraries(simdstr PRIVATE simdstr_search)