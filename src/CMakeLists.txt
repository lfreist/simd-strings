add_subdirectory(search)

add_library(simdstr STATIC simdstr.c)
target_link_libraries(simdstr PRIVATE simdstr_search)