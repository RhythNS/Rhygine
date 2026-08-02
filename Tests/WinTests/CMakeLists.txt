project("WindowsMain")

add_executable(
	WinMain WIN32
	"WinMain.cpp"
)

target_link_libraries(
	WinMain
	PRIVATE Common
)

target_link_libraries(
	WinMain
	LINK_PUBLIC RhyCore
)

RHY_COMPILE_SHADER(
	VERTEX_SPV
	"${CMAKE_CURRENT_SOURCE_DIR}/BasicVert.hlsl"
	"vs_6_0"
	"main"
)

RHY_COMPILE_SHADER(
	PIXEL_SPV
	"${CMAKE_CURRENT_SOURCE_DIR}/BasicPixel.hlsl"
	"ps_6_0"
	"main"
)

add_custom_target(TestShaders DEPENDS ${VERTEX_SPV} ${PIXEL_SPV})
add_dependencies(WinMain TestShaders)
