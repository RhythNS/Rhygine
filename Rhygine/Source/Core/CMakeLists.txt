add_library(
	RhyCore
	"Core/EntryPoint.cpp"
	"Core/EntryPoint.h"
	"Core/MemoryAllocation.h"
	"DataTypes/Concurrency/ILockable.h"
	"DataTypes/Concurrency/Locks.h"
	"DataTypes/Concurrency/Locks.cpp"
	"DataTypes/Concurrency/Mutex.h"
	"DataTypes/Concurrency/Mutex.cpp"
	"DataTypes/Concurrency/Semaphore.h"
	"DataTypes/Concurrency/SharedMutex.h"
	"DataTypes/Concurrency/SharedMutex.cpp"
	"DataTypes/Concurrency/Thread.cpp"
	"DataTypes/Concurrency/Thread.h"
	"Debug/Logger.cpp"
	"Debug/Logger.h"
	"Debug/StackTrace.cpp"
	"Debug/StackTrace.h"
	"Debug/Error.h"
	"Core/Engine.h"
	"Core/Engine.cpp"
	"Config/Config.h"
	"Config/Config.cpp"
  )

target_include_directories (RhyCore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(
	RhyCore
	PRIVATE Common
	PUBLIC spdlog::spdlog_header_only
	PUBLIC glm::glm
	PUBLIC Backward::Interface
	PUBLIC Tracy::TracyClient
	PUBLIC mimalloc-static
	PUBLIC RhyGfx
	PUBLIC RhyIO
	PUBLIC RhySystems
)

if(WIN32)
	target_link_libraries(
		RhyCore
		PUBLIC RhyWindowsSystems
		PUBLIC RhyDx11Gfx
	)
endif()
