add_library(
	RhyIO
	"TomlTest.cpp"
)

target_include_directories (RhyIO PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(
	RhyIO
	PUBLIC RhyCore
	PUBLIC tomlplusplus::tomlplusplus
)
