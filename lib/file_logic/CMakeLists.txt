add_library(file_logic STATIC)
target_include_directories(file_logic PUBLIC
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}
)
target_sources(file_logic PUBLIC
	file_logic.c
)
target_compile_options(file_logic PUBLIC
	-O3
	-Wall -Wextra -Wpedantic -Werror
)
target_link_libraries(file_logic PRIVATE utils)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE file_logic)
