add_library(
	ImGUI
	Include/imgui/imconfig.h
	Include/imgui/imgui.h
	Include/imgui/ImGuiModule.h
	Include/imgui/imgui_impl_dx11.h
	Include/imgui/imgui_impl_win32.h
	Include/imgui/imgui_internal.h
	Include/imgui/imstb_rectpack.h
	Include/imgui/imstb_textedit.h
	Include/imgui/imstb_truetype.h
	Include/Rhyimgui.h
	Source/imgui.cpp
	Source/ImGuiModule.cpp
	Source/imgui_demo.cpp
	Source/imgui_draw.cpp
	Source/imgui_impl_dx11.cpp
	Source/imgui_impl_win32.cpp
	Source/imgui_widgets.cpp
	)
	
target_include_directories (ImGUI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Include")
target_include_directories (ImGUI PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source")

target_link_libraries (
	ImGUI 
	LINK_PUBLIC RhyCore
	)
	