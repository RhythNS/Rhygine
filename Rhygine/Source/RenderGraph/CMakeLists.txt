add_library(
	RhyRenderGraph
	"MaterialDefinition.h"
	"MaterialDefinition.cpp"
	"MaterialCache.h"
	"MaterialCache.cpp"
	"RenderGraph.h"
	"RenderGraph.cpp"
	"RenderGraphTypes.h"
	"RenderPacketBuilder.h"
	"RenderPacketBuilder.cpp"
	"TempGfx.h"
	"TempGfx.cpp"
	"RenderPacket.h"
	"Material.h" 
	"Material.cpp"
	"MaterialParameter.h" 
	"MaterialParameter.cpp"
)

target_include_directories (RhyRenderGraph INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(
	RhyRenderGraph
	PUBLIC RhyCore
	PUBLIC RhyRHI
)
