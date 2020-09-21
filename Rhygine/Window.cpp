#include <string>
#include <thread>
#include <iostream>

#include <strsafe.h>

#include "Window.h"
#include "RhyException.h"

//Window::Window(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow, Scene scene) : hInstance(hInstance), currentScene(scene)
Window::Window(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow) : hInstance(hInstance)
{
	instance = this;
	tickables.push_back(&time);
	tickables.push_back(&mouse);
	tickables.push_back(&keys);

	DWORD dwExStyle = 0;
	LPCSTR lpWindowName = "Window 1";
	DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	int X = 0;
	int Y = 0;
	int nWidth = 1600;
	int nHeight = 900;
	HWND hWndParent = nullptr;
	HMENU hMenu = nullptr;
	LPVOID lpParam = nullptr;

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
			X,
			Y,
			nWidth,
			nHeight,
			hWndParent,
			hMenu,
			hInstance,
			lpParam
		);

	if (windowHandle == nullptr)
		throw RHY_EXCEP("Could not create window!");

	gfx = new Gfx(windowHandle);

	if (ShowWindow(windowHandle, SW_SHOW))
		throw RHY_EXCEP("Could not show window!");
}


Window::~Window()
{
	delete gfx;
}

inline Window* Window::GetInstance()
{
	return instance;
}

inline HWND Window::GetWindowHandle()
{
	return windowHandle;
}

inline HINSTANCE Window::GetHInstance()
{
	return hInstance;
}

int Window::MainLoop()
{
	std::chrono::duration<double> wantedTime = std::chrono::microseconds(1000000 / 144);

	auto startTime = std::chrono::steady_clock::now();
	auto timeBefore = std::chrono::steady_clock::now();

	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
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

		gfx->BeginDraw();
		gfx->EndDraw();

		auto frameTime = timeNow - timeBefore;

		if (frameTime < wantedTime)
			std::this_thread::sleep_for(wantedTime - frameTime);

		//SetWindowTextA(windowHandle, std::to_string(time.GetTimeSinceStart()).c_str());

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
	switch (msg)
	{

		// Keyboard Events ----
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		// Close the program. Only for fast debuging purposes.
		if (wParam == VK_ESCAPE) {
			PostMessageA(hWnd, WM_CLOSE, 0, 0);
			break;
		}
		keys.PressKey((unsigned char) wParam);

		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		keys.ReleaseKey((unsigned char) wParam);
		break;

	case WM_CHAR:
		keys.CharTyped((unsigned char) wParam);
		break;

	case WM_KILLFOCUS:
		keys.ResetCurrentKeys();
		mouse.ResetCurrentKeys();
		break;

		// Mouse Events ----
	case WM_MOUSEMOVE:
		POINTS pos = MAKEPOINTS(lParam);
		mouse.Move(pos.x, pos.y);
		break;

	case WM_LBUTTONDOWN:
		mouse.ButtonClicked(RH_MOUSE_LEFT);
		break;

	case WM_LBUTTONUP:
		mouse.ButtonReleased(RH_MOUSE_LEFT);
		break;

	case WM_RBUTTONDOWN:
		mouse.ButtonClicked(RH_MOUSE_RIGHT);
		break;

	case WM_RBUTTONUP:
		mouse.ButtonReleased(RH_MOUSE_RIGHT);
		break;

	case WM_MBUTTONDOWN:
		mouse.ButtonClicked(RH_MOUSE_MIDDLE);
		break;

	case WM_MBUTTONUP:
		mouse.ButtonReleased(RH_MOUSE_MIDDLE);
		break;

	case WM_XBUTTONDOWN: // mouse button 3, 4
		// Hiword from w param is 1 for xbutton1 and 2 for xbutton2. So just add that to the middle
		// mouse button value (2) to get either (3) for X1 or (4) for X2.
		mouse.ButtonClicked(HIWORD(wParam) + RH_MOUSE_MIDDLE); 
		break;

	case WM_XBUTTONUP:
		// See comment above
		mouse.ButtonReleased(HIWORD(wParam) + RH_MOUSE_MIDDLE);
		break;

	case WM_MOUSEWHEEL:
	{
		int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.Scroll(delta);
		break;
	}

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