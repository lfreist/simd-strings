add_subdirectory(src)

include_directories(include)

install(TARGETS simdstr_search_cpp
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)
install(DIRECTORY include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.hpp"
)