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
#include "Physics.h"
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
	tickables.push_back(&mouse);
	tickables.push_back(&keys);

	// Create all variables needed for the window creation
	DWORD dwExStyle = 0;
	DWORD dwStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SIZEBOX | WS_MAXIMIZEBOX;
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

	// Register class
	windowHandle =
		CreateWindowExA(
			dwExStyle,
			className.c_str(),
			definition.windowName,
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

	gfx = new Gfx(this, definition.targetFramesPerSecond);

	// init imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(windowHandle);
	ImGui_ImplDX11_Init(gfx->device.Get(), gfx->context.Get());

	// registe raw inputs
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

	// Set the target frame time
	if (definition.targetFramesPerSecond > 0)
		time.wantedFrameTime = std::chrono::microseconds(1000000 / definition.targetFramesPerSecond);

	// Init the current scene
	currentScene->Init();

	// If physics should be enabled, then init it now
	if (definition.enablePhysics)
	{
		physics = new Physics(definition.physicsUpdateTime);
		if (definition.physicsStartDebugMode)
			physics->EnableDebug(currentScene->stage.get());
	}

	// Let the user init their scene
	currentScene->InnerInit();

	// Lastly show the created window
	if (ShowWindow(windowHandle, SW_SHOW))
		throw RHY_EXCEP("Could not show window!");

	SetCaptureMouse(definition.mouseCaptured);
}

Window::~Window()
{
	// Destroy imgui
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// gfx was manually created, so delete it
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

void Window::SetCaptureMouse(bool enable)
{
	if (enable)
	{
		capturingMouse = true;
		CaptureMouse();
	}
	else
	{
		capturingMouse = false;
		ClipCursor(NULL);
	}
}

bool Window::IsCapturingMouse()
{
	return capturingMouse;
}

void Window::SetFullscreen(bool enable)
{
	// Want to fullscreen
	if (enable)
	{
		// Already in fullscren?
		if (inFullscreen)
			return;

		inFullscreen = true;

		// Get the monitor and monitor info
		HMONITOR Monitor = MonitorFromWindow(windowHandle, MONITOR_DEFAULTTONEAREST);
		MONITORINFO MonitorInfo = { sizeof(MonitorInfo) };
		if (!(GetMonitorInfo(Monitor, &MonitorInfo)))
			throw RHY_EXCEP("Could not get monitor info!");

		// Save the previous window position
		GetWindowRect(windowHandle, &beforeFullscreenWindowPos);

		// Set the window as a borderless window
		SetWindowLongPtr(windowHandle, GWL_STYLE, WS_POPUP | WS_VISIBLE);

		// Resize to fill monitor
		Resize(
			MonitorInfo.rcMonitor.left,
			MonitorInfo.rcMonitor.top,
			MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left,
			MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top
		);

		return;
	}
	// Not fullscreen

	// Already in fullscreen
	if (!inFullscreen)
		return;

	inFullscreen = false;

	// Set normal window
	SetWindowLongPtr(windowHandle, GWL_STYLE, WS_CAPTION | WS_MINIMIZEBOX | WS_SIZEBOX | WS_MAXIMIZEBOX);

	// Resize back to before it went into fullscreen
	Resize(
		beforeFullscreenWindowPos.left,
		beforeFullscreenWindowPos.top,
		beforeFullscreenWindowPos.right - beforeFullscreenWindowPos.left,
		beforeFullscreenWindowPos.bottom - beforeFullscreenWindowPos.top
	);
}

bool Window::IsInFullscreen()
{
	return inFullscreen;
}

void Window::Resize(int posX, int posY, int width, int height)
{
	SetWindowPos( windowHandle, 0, posX, posY, width, height, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

int Window::MainLoop()
{
	MSG msg;
	// Main loop start
	while (true)
	{
		// First go through all recieved messages
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// if the message was a quit message, return the wParam as an exit code.
			if (msg.message == WM_QUIT)
				return (int)msg.wParam;

			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		time.StartOfFrame();

		// Go through all tickables that need to be updated.
		for (Tickable* i : tickables)
		{
			i->Tick();
		}

		// If physics are enabled, then simulate the physics world
		if (physics)
		{
			bool moved = false;
			while (time.ShouldUpdatePhysics())
			{
				physics->Tick();
				moved = true;
			}
			if (moved)
				physics->UpdatePositions();
		}

		// Prepare imgui for a new frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// Update the scene
		currentScene->Update();

		// Draw the scene
		gfx->BeginDraw();
		currentScene->Draw();

		// If physics is enabled and the debug mode is enabled then draw the wireframes of
		// the physics objects.
		if (physics && physics->IsDebugEnabled())
		{
			gfx->ClearDepth();
			physics->DebugDraw();
		}

		// Draw everything and present the frame.
		gfx->EndDraw();

		// Set the frame time and sleep if needed.
		time.EndOfFrame();
		std::chrono::duration<float> sleepTime = time.GetSleepTime();
		if (sleepTime.count() > 0.0f)
			std::this_thread::sleep_for(sleepTime);
	}
}

void Window::AddTickable(Tickable* tickable)
{
	assert(tickable);
	tickables.push_back(tickable);
}

LRESULT CALLBACK Window::ProcessPassthrough(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return instance->ProcessMessage(hWnd, msg, wParam, lParam);
}

LRESULT Window::ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Let ImGUI handle the message first
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	// switch case with all implemented message types
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

		// Is it for the keyboard?
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
				if (raw->data.keyboard.VKey == VK_ESCAPE)
				{
					PostMessageA(hWnd, WM_CLOSE, 0, 0);
					break;
				}

				else if (raw->data.keyboard.VKey == VK_F11)
				{
					SetFullscreen(!inFullscreen);
				}

				keys.PressKey(raw->data.keyboard.VKey);
			}
		}
		// Is it for the mouse?
		else if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			// Is it a relative movement?
			if (((raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE) != MOUSE_MOVE_ABSOLUTE) &&
				(raw->data.mouse.lLastX != 0 || raw->data.mouse.lLastY != 0))
			{
				mouse.RelativeMove((int)raw->data.mouse.lLastX, (int)raw->data.mouse.lLastY);
			}

			// Check if any buttons where clicked/ released
			switch (raw->data.mouse.usButtonFlags)
			{
			case RI_MOUSE_LEFT_BUTTON_DOWN:
				mouse.ButtonClicked(RH_MOUSE_LEFT);
				currentScene->OnMouseDown();
				break;
			case RI_MOUSE_LEFT_BUTTON_UP:
				mouse.ButtonReleased(RH_MOUSE_LEFT);
				currentScene->OnMouseUp();
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

		// delete the allocated input data
		delete[] lpb;
		break;
	}

	case WM_MOUSEMOVE: // Get the absolute mouse position
		POINTS pos = MAKEPOINTS(lParam);
		mouse.AbsoluteMove(pos.x, pos.y);
		currentScene->OnMouseMove(pos.x, pos.y);
		break;

	case WM_CHAR: // Get chars for the keyboard queue.
		keys.CharTyped((unsigned char)wParam);
		break;

	case WM_KILLFOCUS: // When the window goes out of focus
		// Reset all keys and the mouse so that no keys are stuck being pressed.
		keys.ResetCurrentKeys();
		mouse.ResetCurrentKeys();
		// Unclip the cursor.
		ClipCursor(NULL);
		break;

	case WM_SETFOCUS: // When the window gains focus
		if (capturingMouse)
			CaptureMouse();
		break;

		// Quit Events ----
	case WM_CLOSE:
		// insert auto save stuff and a display like: "do you really want to close the program?"
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SIZE: // When the window was resized
		width = LOWORD(lParam);
		height = HIWORD(lParam);

		gfx->OnResize(width, height);
		currentScene->OnResize(width, height);
		break;

	case WM_SIZING:
		// Maybe pause the game?
		break;
	}

	// return the default window proc, to make the window responsive.
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Window::CaptureMouse()
{
	// Get the current client size and clip the cursor to it.
	WINDOWINFO windowinfo;
	windowinfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(windowHandle, &windowinfo);

	ClipCursor(&windowinfo.rcClient);
}

// init of static fields
Window* Window::instance;

std::string Window::className = "Rhygine";
