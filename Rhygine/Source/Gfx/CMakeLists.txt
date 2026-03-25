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

option(ENABLE_DX11 "Enable DirectX 11 support" OFF)

if(ENABLE_DX11)
    add_subdirectory("DX11")
endif()
