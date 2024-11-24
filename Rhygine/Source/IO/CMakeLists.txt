add_library(
	RhyIO
	"TomlTest.cpp"
	"File/File.cpp"
	"File/File.h"
	"File/StandardFile.cpp"
	"File/StandardFile.h"
	"Interpreters/TomlInterpreter.h" 
	"Interpreters/TomlInterpreter.cpp"
	"File/FileManager.h"
	"File/FileManager.cpp"
	"File/FileProvider.h"
	"File/FileProvider.cpp"
	"File/StandardFileProvider.h"
	"File/StandardFileProvider.cpp"
)

target_include_directories (RhyIO PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(
	RhyIO
	PUBLIC RhyCore
	PUBLIC tomlplusplus::tomlplusplus
)
