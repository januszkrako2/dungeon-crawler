target_sources(${CMAKE_PROJECT_NAME} PRIVATE
	main.c
	game.c
	file.c
	utility.c
)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/include
)
