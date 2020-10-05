#include "Testimgui.h"
#include "Rhyimgui.h"
#include "Window.h"

void Testimgui::Init()
{
}

void Testimgui::Update()
{
    if (Window::GetInstance()->keys.IsKeyDownThisFrame(VK_F1))
        show = !show;
    if (show)
        ImGui::ShowDemoWindow(&show);
}

void Testimgui::Draw()
{
}
