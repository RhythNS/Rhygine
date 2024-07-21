add_subdirectory("Backward")

set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "Build examples")
set(SPDLOG_BUILD_TESTING OFF CACHE BOOL "Build spdlog tests")
add_subdirectory("Spdlog")

option(TRACY_ENABLE "" ON)
option(TRACY_ON_DEMAND "" ON)
add_subdirectory(Tracy)
