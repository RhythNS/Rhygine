add_library(
	RhyWindowsSystems
	"RhyWindows.h"
	"WindowsSystem.h"
	"WindowsSystem.cpp"
	"WindowsWindow.h"
	"WindowsWindow.cpp"
	"IWindowsMessageHandler.h"
)

target_link_libraries(
	RhyWindowsSystems
	PUBLIC RhySystems
)
