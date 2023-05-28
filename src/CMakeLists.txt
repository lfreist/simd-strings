add_library(simdstr simdstr.c search.c)
target_compile_options(simdstr PUBLIC "-mavx2")