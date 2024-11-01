project("WindowsMain")

add_executable(
	WinMain WIN32
	"WinMain.cpp"
)

target_link_libraries(
	WinMain
	PRIVATE Common
)

target_link_libraries(
	WinMain
	LINK_PUBLIC RhyCore
)
