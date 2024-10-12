add_library(
	Rhygine
	"Core/EntryPoint.cpp"
	"Core/EntryPoint.h"
	"Core/MemoryAllocation.h"
	"DataTypes/Concurrency/Thread.cpp"
	"DataTypes/Concurrency/Thread.h"
	"Debug/Logger.cpp"
	"Debug/Logger.h"
	"Debug/StackTrace.cpp"
	"Debug/StackTrace.h"
)

target_include_directories (Rhygine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(
	Rhygine
	PRIVATE Common
	PRIVATE spdlog::spdlog_header_only
	PUBLIC Backward::Interface
	PUBLIC Tracy::TracyClient
	PUBLIC mimalloc-static
)
