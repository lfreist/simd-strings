find_package(Python3 COMPONENTS Development REQUIRED)

add_library(py_simdstr MODULE py_simd_strings.c)

if (WIN32)
    set_target_properties(py_simdstr PROPERTIES PREFIX "")
    set_target_properties(py_simdstr PROPERTIES SUFFIX ".pyd")
else ()
    set_target_properties(py_simdstr PROPERTIES PREFIX "")
    set_target_properties(py_simdstr PROPERTIES SUFFIX ".so")
endif ()

target_link_libraries(py_simdstr PRIVATE ${Python3_LIBRARIES} simdstr_search)
target_include_directories(py_simdstr PRIVATE ${Python3_INCLUDE_DIRS})
