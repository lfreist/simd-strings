add_subdirectory(utils)

add_library(simdstr_search search.c)
target_link_libraries(simdstr_search PUBLIC utils)
target_compile_options(simdstr_search PUBLIC "-mavx2" "-mavx512f" "-mavx512bw")

add_library(fat_teddy fat_teddy.c)
target_link_libraries(fat_teddy PUBLIC utils)
target_compile_options(fat_teddy PUBLIC "-mavx2")
