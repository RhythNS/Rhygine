add_subdirectory("Backward")

set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "Build examples")
set(SPDLOG_BUILD_TESTING OFF CACHE BOOL "Build spdlog tests")
add_subdirectory("Spdlog")

option(TRACY_ENABLE "" ON)

option(TRACY_NO_LOGS "Tracy does not recieve logs as messages" OFF)
if(TRACY_NO_LOGS)
    add_compile_definitions(TRACY_NO_LOG) 
endif()

add_subdirectory(Tracy)
