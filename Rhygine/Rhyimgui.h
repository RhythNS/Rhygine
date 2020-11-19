#pragma once

// Includer for including imgui classes.

#include "RhyWin.h"


// Ignore imgui compiler warnings
#pragma warning(push)
#pragma warning(disable : 26495)

#include "imgui\imgui.h"
#include "imgui\imgui_impl_win32.h"
#include "imgui\imgui_impl_dx11.h"

#pragma warning(pop)

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
