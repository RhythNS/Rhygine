add_library(
	RhyWindowsSystems
	"Windows.h"
	"WindowsSystem.h"
	"WindowsSystem.cpp"
 "WindowsWindow.h" "WindowsWindow.cpp" "IWindowsMessageHandler.h")

target_link_libraries(
	RhyWindowsSystems
	PUBLIC RhySystems
)
