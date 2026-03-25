#include "WindowsSystem.h"

#include <iostream>
#include <tracy/Tracy.hpp>

#include "Gfx.h"
#include "Config/Config.h"
#include "Debug/Logger.h"

Rhygine::WindowsSystem* Rhygine::WindowsSystem::s_instance = nullptr;

Rhygine::WindowsSystem::WindowsSystem(HINSTANCE t_instance, HINSTANCE t_prev_instance, Config& t_config) :
	System(t_config), m_instance(t_instance), m_prev_instance(t_prev_instance)
{
	ASSERT_ERROR_MESSAGE(!s_instance, "Another WindowsSystem has already been created!");
	s_instance = this;

	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(windowClass);
	windowClass.style = CS_OWNDC;
	windowClass.lpfnWndProc = ProcessPassthrough;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = m_instance;
	windowClass.hIcon = nullptr;
	windowClass.hCursor = nullptr;
	windowClass.hbrBackground = nullptr;
	windowClass.lpszMenuName = nullptr;
	windowClass.lpszClassName = m_window_class_name.c_str();
	windowClass.hIconSm = nullptr;
	RegisterClassEx(&windowClass);
}

Rhygine::WindowsSystem::~WindowsSystem()
{
	s_instance = nullptr;
}

Rhygine::WindowsSystem* Rhygine::WindowsSystem::GetInstance()
{
	return s_instance;
}

std::optional<int> Rhygine::WindowsSystem::ProcessMessages()
{
	ZoneScoped;

	MSG msg;
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		// if the message was a quit message, return the wParam as an exit code.
		if (msg.message == WM_QUIT)
			return (int)msg.wParam;

		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	return {};
}

bool Rhygine::WindowsSystem::CreateConsole()
{
	ZoneScoped;

	bool createdConsole = AllocConsole();

	if (!createdConsole)
	{
		LOG_ERROR(GetLastSystemError());
		return false;
	}

	FILE* temp;
	freopen_s(&temp, "CONOUT$", "w", stdout);
	freopen_s(&temp, "CONOUT$", "w", stderr);
	std::cout.clear();
	std::clog.clear();
	std::cerr.clear();

	HANDLE hConOut = CreateFile(TEXT("CONOUT$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
	SetStdHandle(STD_ERROR_HANDLE, hConOut);
	std::wcout.clear();
	std::wclog.clear();
	std::wcerr.clear();

	Logger::GetInstance()->ResetToDefaultLogger();
	return true;
}

bool Rhygine::WindowsSystem::DestroyConsole()
{
	ZoneScoped;

	bool freedConsole = FreeConsole();

	if (!freedConsole)
	{
		LOG_ERROR(GetLastSystemError());
		return false;
	}

	return true;
}

Rhygine::Window::WindowId Rhygine::WindowsSystem::AddWindow()
{
	ZoneScoped;

	DWORD dwExStyle = 0;
	// DWORD dwStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SIZEBOX | WS_MAXIMIZEBOX;
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	HWND hWndParent = nullptr;
	HMENU hMenu = nullptr;
	LPVOID lpParam = nullptr;

	// TODO: Set this properly
	int x = 100, y = 100, width = 1920, height = 1080;
	std::string windowTitle = "Rhygine";

	RECT wr;
	wr.left = x;
	wr.right = width + x;
	wr.top = y;
	wr.bottom = height + y;

	bool adjustedWindow = AdjustWindowRect(&wr, dwStyle, FALSE);
	if (!adjustedWindow)
	{
		LOG_ERROR(GetLastSystemError());
	}

	HWND windowHandle =
		CreateWindowExA(
			dwExStyle,
			m_window_class_name.c_str(),
			windowTitle.c_str(),
			dwStyle,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			wr.right - wr.left,
			wr.bottom - wr.top,
			hWndParent,
			hMenu,
			m_instance,
			lpParam
		);

	// TODO: remove
	{
		static bool primary = true;
		static Window::WindowId ids = 0;
		m_windows.push_back(std::make_unique<WindowsWindow>(ids++, primary, x, y, width, height, windowHandle));
		primary = false;
		WindowsWindow* window = m_windows.back().get();
		Gfx::GetInstance()->OnWindowAdded(window);
		ShowWindow(windowHandle, SW_SHOW);
	}


	return Window::WindowId();
}

bool Rhygine::WindowsSystem::DestroyWindow(Window::WindowId t_id)
{
	ZoneScoped;

	// TODO: implementation

	return false;
}

bool Rhygine::WindowsSystem::AddMessageHandler(IWindowsMessageHandler* t_message_handler)
{
	if (std::find(m_message_handlers.begin(), m_message_handlers.end(), t_message_handler) != m_message_handlers.end())
	{
		LOG_ERROR("MessageHandler already in list. Not adding again!");
		return false;
	}

	m_message_handlers.push_back(t_message_handler);
	return true;
}

bool Rhygine::WindowsSystem::RemoveMessageHandler(IWindowsMessageHandler* t_message_handler)
{
	auto iterator = std::find(m_message_handlers.begin(), m_message_handlers.end(), t_message_handler);
	if (iterator == m_message_handlers.end())
	{
		LOG_ERROR("MessageHandler was not found in list. Can not remove it!");
		return false;
	}

	m_message_handlers.erase(iterator);
	return true;
}

std::string Rhygine::WindowsSystem::GetLastSystemError()
{
	ZoneScoped;

	DWORD errorMessageID = GetLastError();
	if (errorMessageID == 0)
	{
		return std::string();
	}

	LPSTR messageBuffer = nullptr;

	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	LocalFree(messageBuffer);

	return message;
}

LRESULT Rhygine::WindowsSystem::ProcessPassthrough(HWND t_window, UINT t_message, WPARAM t_w_param, LPARAM t_l_param)
{
	return s_instance->ProcessMessage(t_window, t_message, t_w_param, t_l_param);
}

LRESULT Rhygine::WindowsSystem::ProcessMessage(HWND t_window, UINT t_message, WPARAM t_w_param, LPARAM t_l_param)
{
	ZoneScoped;

	for (IWindowsMessageHandler* messageHandler : m_message_handlers)
	{
		if (messageHandler->ProcessMessage(t_window, t_message, t_w_param, t_l_param))
		{
			return true;
		}
	}

	// TODO: messages

	return DefWindowProc(t_window, t_message, t_w_param, t_l_param);
}
