add_library(
	RhyDx11Gfx
	"DX11.cpp"
	"DX11.h"
	"DX11Screen.cpp"
	"DX11Screen.h"
)

target_link_libraries(
	RhyDx11Gfx
	PUBLIC RhyGfx
)
