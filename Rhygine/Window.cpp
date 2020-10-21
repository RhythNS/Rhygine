#include "Window.h"

#include <thread>
#include <iostream>
#include <string>

#include "RhyException.h"
#include "Rhyimgui.h"
#include "Keys.h"
#include "Time.h"
#include "Mouse.h"
#include "Gfx.h"
#include "Scene.h"
#include "Tickable.h"

Window::Window(WindowDefinition definition) :
	hInstance(definition.hInstance),
	currentScene(definition.startScene),
	width(definition.width),
	height(definition.height)
{
	instance = this;
	currentScene->window = this;
	tickables.push_back(&time);
	tickables.push_back(&mouse);
	tickables.push_back(&keys);

	DWORD dwExStyle = 0;
	LPCSTR lpWindowName = "Window 1";
	DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	HWND hWndParent = nullptr;
	HMENU hMenu = nullptr;
	LPVOID lpParam = nullptr;

	RECT wr;
	wr.left = definition.left;
	wr.right = definition.width + wr.left;
	wr.top = definition.top;
	wr.bottom = definition.height + wr.top;

	if (!AdjustWindowRect(&wr, dwStyle, FALSE))
		throw RHY_EXCEP("Could not adjust window rect!");

	// Register class
	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(windowClass);
	windowClass.style = CS_OWNDC;
	windowClass.lpfnWndProc = ProcessPassthrough;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = nullptr;
	windowClass.hCursor = nullptr;
	windowClass.hbrBackground = nullptr;
	windowClass.lpszMenuName = nullptr;
	windowClass.lpszClassName = className.c_str();
	windowClass.hIconSm = nullptr;
	RegisterClassEx(&windowClass);

	windowHandle =
		CreateWindowExA(
			dwExStyle,
			className.c_str(),
			lpWindowName,
			dwStyle,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			wr.right - wr.left,
			wr.bottom - wr.top,
			hWndParent,
			hMenu,
			hInstance,
			lpParam
		);

	if (windowHandle == nullptr)
		throw RHY_EXCEP("Could not create window!");

	gfx = new Gfx(this);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(windowHandle);
	ImGui_ImplDX11_Init(gfx->device.Get(), gfx->context.Get());

	RAWINPUTDEVICE Rid[2];
	// Add mouse
	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x02;
	Rid[0].dwFlags = 0; // RIDEV_NOLEGACY;
	Rid[0].hwndTarget = 0;

	// Add keyboard
	Rid[1].usUsagePage = 0x01;
	Rid[1].usUsage = 0x06;
	Rid[1].dwFlags = 0;// RIDEV_NOLEGACY;
	Rid[1].hwndTarget = 0;

	if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE)
		throw RHY_EXCEP("Could not register for raw input!");

	if (ShowWindow(windowHandle, SW_SHOW))
		throw RHY_EXCEP("Could not show window!");

	currentScene->Init();
}

Window::~Window()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	delete gfx;
}

Window* Window::GetInstance()
{
	return instance;
}

Gfx* Window::GetGfx()
{
	return gfx;
}

Scene* Window::GetCurrentScene()
{
	return currentScene;
}

HWND* Window::GetWindowHandle()
{
	return &windowHandle;
}

HINSTANCE* Window::GetHInstance()
{
	return &hInstance;
}

int Window::GetWidth()
{
	return width;
}

int Window::GetHeight()
{
	return height;
}

void Window::SetTitle(LPCSTR lpString)
{
	SetWindowTextA(windowHandle, lpString);
}

int Window::MainLoop()
{
	std::chrono::duration<double> wantedTime = std::chrono::microseconds(1000000 / 144);

	auto startTime = std::chrono::steady_clock::now();
	auto timeBefore = std::chrono::steady_clock::now();

	MSG msg;
	while (true)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				return (int)msg.wParam;

			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		auto timeNow = std::chrono::steady_clock::now();
		std::chrono::duration<float> totalElapsed = timeNow - startTime;

		for (Tickable* i : tickables)
		{
			i->Tick();
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		currentScene->Update();
		gfx->BeginDraw();
		currentScene->Draw();
		gfx->EndDraw();

		auto frameTime = timeNow - timeBefore;

		if (frameTime < wantedTime)
			std::this_thread::sleep_for(wantedTime - frameTime);

		timeBefore = timeNow;
	}
}

void Window::AddTickable(Tickable* tickable)
{
	tickables.push_back(tickable);
}

LRESULT CALLBACK Window::ProcessPassthrough(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return instance->ProcessMessage(hWnd, msg, wParam, lParam);
}

LRESULT Window::ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
		// ---------- TAKEN FROM microsoft docs -------------------------
		// https://docs.microsoft.com/de-de/windows/win32/inputdev/using-raw-input#registering-for-raw-input
	case WM_INPUT:
	{
		HRESULT hResult;
		UINT dwSize;

		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
		LPBYTE lpb = new BYTE[dwSize];
		if (lpb == NULL)
		{
			break;
		}

		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

		RAWINPUT* raw = (RAWINPUT*)lpb;
		// ---------- TAKEN FROM microsoft docs -------------------------

		if (raw->header.dwType == RIM_TYPEKEYBOARD)
		{
			// raw->data.keyboard
			if (raw->data.keyboard.Flags & RI_KEY_BREAK)
			{
				keys.ReleaseKey(raw->data.keyboard.VKey);
			}
			else
			{
				// Close the program. Only for fast debuging purposes.
				if (raw->data.keyboard.VKey == VK_ESCAPE) {
					PostMessageA(hWnd, WM_CLOSE, 0, 0);
					break;
				}

				keys.PressKey(raw->data.keyboard.VKey);
			}
		}
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			if ( ((raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE) != MOUSE_MOVE_ABSOLUTE) &&
				(raw->data.mouse.lLastX != 0 || raw->data.mouse.lLastY != 0) )
			{
				mouse.RelativeMove((int)raw->data.mouse.lLastX, (int)raw->data.mouse.lLastY);
			}

			// raw->data.mouse
			switch (raw->data.mouse.usButtonFlags)
			{
			case RI_MOUSE_LEFT_BUTTON_DOWN:
				mouse.ButtonClicked(RH_MOUSE_LEFT);
				break;
			case RI_MOUSE_LEFT_BUTTON_UP:
				mouse.ButtonReleased(RH_MOUSE_LEFT);
				break;
			case RI_MOUSE_MIDDLE_BUTTON_DOWN:
				mouse.ButtonClicked(RH_MOUSE_MIDDLE);
				break;
			case RI_MOUSE_MIDDLE_BUTTON_UP:
				mouse.ButtonReleased(RH_MOUSE_MIDDLE);
				break;
			case RI_MOUSE_RIGHT_BUTTON_DOWN:
				mouse.ButtonClicked(RH_MOUSE_RIGHT);
				break;
			case RI_MOUSE_RIGHT_BUTTON_UP:
				mouse.ButtonReleased(RH_MOUSE_RIGHT);
				break;
			case RI_MOUSE_BUTTON_4_DOWN:
				mouse.ButtonClicked(RH_MOUSE_X1);
				break;
			case RI_MOUSE_BUTTON_4_UP:
				mouse.ButtonReleased(RH_MOUSE_X1);
				break;
			case RI_MOUSE_BUTTON_5_DOWN:
				mouse.ButtonClicked(RH_MOUSE_X2);
				break;
			case RI_MOUSE_BUTTON_5_UP:
				mouse.ButtonReleased(RH_MOUSE_X2);
				break;
			case RI_MOUSE_WHEEL:
				mouse.VertScroll((float)(short)raw->data.mouse.usButtonData);
				break;
			case RI_MOUSE_HWHEEL:
				mouse.HoriScroll((float)(short)raw->data.mouse.usButtonData);
				break;
			default:
				break;
			}
		}

		delete[] lpb;
		break;
	}

	case WM_MOUSEMOVE:
		POINTS pos = MAKEPOINTS(lParam);
		mouse.AbsoluteMove(pos.x, pos.y);
		break;

	case WM_CHAR:
		keys.CharTyped((unsigned char)wParam);
		break;

	case WM_KILLFOCUS:
		keys.ResetCurrentKeys();
		mouse.ResetCurrentKeys();
		ClipCursor(NULL);
		break;

	case WM_SETFOCUS:
		WINDOWINFO windowinfo;

		windowinfo.cbSize = sizeof(WINDOWINFO);
		GetWindowInfo(hWnd, &windowinfo);

		ClipCursor(&windowinfo.rcClient);
		break;

	// Quit Events ----
	case WM_CLOSE:
		// insert auto save stuff and a display like: "do you really want to close the program?"
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window* Window::instance;

std::string Window::className = "Rhygine";
