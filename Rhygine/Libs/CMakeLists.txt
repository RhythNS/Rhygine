add_subdirectory("Backward")

add_subdirectory("Mimalloc")

set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "Build examples")
set(SPDLOG_BUILD_TESTING OFF CACHE BOOL "Build spdlog tests")
add_subdirectory("Spdlog")

option(TRACY_ENABLE "" ON)

option(TRACY_NO_LOGS "Tracy does not recieve logs as messages" ON)
if(TRACY_NO_LOGS)
    target_compile_definitions(Common INTERFACE TRACY_NO_LOG) 
endif()

option(TRACY_NO_LOCKS "Tracy does not track mutex locks" OFF)
if (TRACY_NO_LOCKS)
    target_compile_definitions(Common INTERFACE TRACY_NO_LOCKS) 
endif()

option(TRACY_MEMORY_EVERYTHING "Tracy tracks each memory de-/allocation" OFF)
if (TRACY_MEMORY_EVERYTHING)
    target_compile_definitions(Common INTERFACE TRACY_MEMORY_EVERYTHING)
endif()

add_subdirectory(Tracy)
