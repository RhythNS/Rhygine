add_library(
	RhyGfx
	"Gfx.h"
	"Gfx.cpp"
)

target_include_directories (RhyGfx PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(
	RhyGfx
	PUBLIC RhyCore
)

# TODO:
add_subdirectory("DX11")
