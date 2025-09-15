add_library(text_utils STATIC)
target_include_directories(text_utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)
target_sources(text_utils PUBLIC
    text_utils.c
)
target_compile_options(text_utils PUBLIC
    -O3
    -Wall -Wextra -Wpedantic -Werror
)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE text_utils)
