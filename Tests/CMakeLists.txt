project("Tests")

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
	LINK_PUBLIC Rhygine
)
