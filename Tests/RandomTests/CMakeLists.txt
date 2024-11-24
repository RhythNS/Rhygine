project("RandomTests")

add_executable(
	CCTests
	"Test.cpp"
)

target_link_libraries(
	CCTests
	PRIVATE Common
)

target_link_libraries(
	CCTests
	LINK_PUBLIC RhyCore
)

configure_file(
    "Test.toml"
    "${CMAKE_CURRENT_BINARY_DIR}/res/Test.toml"
    COPYONLY
)