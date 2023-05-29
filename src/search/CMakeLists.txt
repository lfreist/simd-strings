add_library(simdstr_search STATIC search.c)
target_compile_options(simdstr_search PUBLIC "-mavx2")