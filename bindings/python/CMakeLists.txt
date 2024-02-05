find_package(Python3 COMPONENTS Development REQUIRED)

add_library(simdstr MODULE simd_strings.c)

set_target_properties(simdstr PROPERTIES PREFIX "")
set_target_properties(simdstr PROPERTIES SUFFIX ".pyd")

target_link_libraries(simdstr PRIVATE ${Python3_LIBRARIES} simdstr_search)
target_include_directories(simdstr PRIVATE ${Python3_INCLUDE_DIRS})
