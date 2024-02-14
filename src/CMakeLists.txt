add_subdirectory(utils)

add_library(simdstr_search search.c)
target_link_libraries(simdstr_search PUBLIC utils)
target_compile_options(simdstr_search PUBLIC "-mavx2" "-mavx512f" "-mavx512vl" "-mavx512bw")

add_library(teddy teddy.c)
target_link_libraries(teddy PUBLIC utils)
target_compile_options(teddy PUBLIC "-mavx2")
