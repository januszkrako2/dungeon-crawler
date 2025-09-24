add_library(game_logic STATIC)
target_include_directories(game_logic PUBLIC
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}
)
target_sources(game_logic PUBLIC
	game_logic.c
)
target_compile_options(game_logic PUBLIC
	-O3
	-Wall -Wextra -Wpedantic -Werror
)
target_link_libraries(game_logic PRIVATE utils)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE game_logic)
