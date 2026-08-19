add_library(
	RhyCore
	"Core/EntryPoint.cpp"
	"Core/EntryPoint.h"
	"Core/MemoryAllocation.h"
	"DataTypes/Math.h"
	"DataTypes/Concurrency/Mutex.h"
	"DataTypes/Concurrency/Mutex.cpp"
	"DataTypes/Concurrency/Semaphore.h"
	"DataTypes/Concurrency/SingleConcurrentQueue.h"
	"DataTypes/Concurrency/SingleConcurrentQueue.cpp"
	"DataTypes/Concurrency/SharedMutex.h"
	"DataTypes/Concurrency/SharedMutex.cpp"
	"DataTypes/Concurrency/Thread.cpp"
	"DataTypes/Concurrency/Thread.h"
	"DataTypes/Math/AABB.h"
	"Debug/Logger.cpp"
	"Debug/Logger.h"
	"Debug/StackTrace.cpp"
	"Debug/StackTrace.h"
	"Debug/Error.h"
	"Config/Config.h"
	"Config/Config.cpp"
	"DataTypes/Math/AABB.cpp" 
	"DataTypes/Concurrency.h")

target_include_directories (RhyCore PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/Rhygine/Source"
)

target_compile_features(RhyCore PUBLIC cxx_std_23)

target_link_libraries(
	RhyCore
	PRIVATE Common
	PUBLIC spdlog::spdlog_header_only
	PUBLIC glm::glm
	PUBLIC Backward::Interface
	PUBLIC Tracy::TracyClient
	PUBLIC mimalloc-static
	PUBLIC readerwriterqueue
	PUBLIC RhyIO
	PUBLIC RhySystems
	PUBLIC RhyWorld
	PUBLIC RhyRHI
	PUBLIC RhyRenderGraph
)

if(WIN32)
	target_link_libraries(
		RhyCore
		PUBLIC RhyWindowsSystems
	)
endif()

# Not sure if there is a better way to do this...
# It would be nice to have a way to automatically link the RHI implementation based on the build configuration
if(ENABLE_VULKAN)
	target_link_libraries(RhyCore PUBLIC RhyRHIVulkan)
endif()
