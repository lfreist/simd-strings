add_library(statistics statistics.c)

if (NOT WIN32)
    target_link_libraries(statistics PRIVATE m)
endif ()