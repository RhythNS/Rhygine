add_library(
	RhyIO
	"TomlTest.cpp"
	"File/File.cpp"
	"File/File.h"
	"File/StandardFile.cpp"
	"File/StandardFile.h"
)

target_include_directories (RhyIO PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(
	RhyIO
	PUBLIC RhyCore
	PUBLIC tomlplusplus::tomlplusplus
)
