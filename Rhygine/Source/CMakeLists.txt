add_library(
	Rhygine
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
