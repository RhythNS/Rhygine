#include "ImGuiModule.h"
#include "Rhyimgui.h"
#include "Window.h"
#include "Gfx.h"

ImGuiModule::~ImGuiModule()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool ImGuiModule::ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}

void ImGuiModule::Setup()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(GetWindow()->GetWindowHandle());
	ImGui_ImplDX11_Init(GetDevice().Get(), GetContext().Get());
}

void ImGuiModule::StartOfFramePreUpdate()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}
