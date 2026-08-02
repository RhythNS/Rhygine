add_library(
	RhyRenderGraph
	"TempGfx.h"
	"TempGfx.cpp"
)

target_include_directories (RhyRenderGraph INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(
	RhyRenderGraph
	PUBLIC RhyCore
	PUBLIC RhyRHI
)

if(ENABLE_VULKAN)
	target_link_libraries(RhyRenderGraph PUBLIC RhyRHIVulkan)
endif()
