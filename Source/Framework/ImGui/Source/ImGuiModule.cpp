#include <imgui/ImGuiModule.h>
#include <Rhyimgui.h>
#include <Core/Window.h>
#include <Core/Gfx.h>

ImGuiModule::~ImGuiModule()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	GetWindow()->RemoveMessageHandler(this);
}

bool ImGuiModule::ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}

void ImGuiModule::Setup()
{
	GetWindow()->AddMessageHandler(this);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplWin32_Init(GetWindow()->GetWindowHandle());
	ImGui_ImplDX11_Init(GetDevice().Get(), GetContext().Get());

	ImGui::StyleColorsDark();
}

void ImGuiModule::StartOfFramePreUpdate()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiModule::LateDraw()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
