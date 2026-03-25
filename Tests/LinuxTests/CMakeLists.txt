project("LinuxMain")
	
add_executable(
	LinuxMain
	"LinuxMain.cpp"
)

target_link_libraries(
	LinuxMain
	PRIVATE Common
)

target_link_libraries(
	LinuxMain
	LINK_PUBLIC RhyCore
)
