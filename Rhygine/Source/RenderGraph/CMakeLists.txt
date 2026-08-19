add_library(
	RhyRenderGraph
	"MaterialDefinition.h"
	"MaterialDefinition.cpp"
	"MaterialCache.h"
	"MaterialCache.cpp"
	"RenderGraph.h"
	"RenderGraph.cpp"
	"RenderGraphTypes.h"
	"RenderGraphBuilder.h"
	"RenderGraphBuilder.cpp"
	"RenderGraphResources.h"
	"RenderGraphResources.cpp"
	"RenderGraphCompiler.h"
	"RenderGraphCompiler.cpp"
	"TransientResourcePool.h"
	"TransientResourcePool.cpp"
	"DeletionQueue.h"
	"DeletionQueue.cpp"
	"TripleBufferedPackets.h"
	"RenderPacketBuilder.h"
	"RenderPacketBuilder.cpp"
	"RenderPacket.h"
	"Material.h"
	"Material.cpp"
	"MaterialParameter.h"
	"MaterialParameter.cpp"
	"SceneRenderer.h"
	"SceneRenderer.cpp"
	"PassModules/IPassModule.h"
	"Debug/RenderGraphDebugInfo.h"
	"Debug/RenderGraphVisualizer.h"
	"Debug/RenderGraphVisualizer.cpp"
	"TempGfx.h"
	"TempGfx.cpp"
)

target_include_directories(RhyRenderGraph INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(
	RhyRenderGraph
	PUBLIC RhyCore
	PUBLIC RhyRHI
)
