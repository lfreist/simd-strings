add_library(simdstr_search_cpp SHARED search.cpp)
target_link_libraries(simdstr_search_cpp PUBLIC simdstr_search)
target_include_directories(simdstr_search_cpp PUBLIC ../include)