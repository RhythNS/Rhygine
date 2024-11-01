add_library(
	RhySystems
	"System.h"
	"System.cpp"
 "Window.h" "Window.cpp")

target_include_directories (RhySystems PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(
	RhySystems
	PUBLIC RhyCore
)

if(WIN32)
	add_subdirectory("Windows")
endif()
