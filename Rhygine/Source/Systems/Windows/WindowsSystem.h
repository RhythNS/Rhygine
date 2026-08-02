#pragma once

#include <string>
#include <vector>
#include <memory>

#include "RhyWindows.h"
#include "IWindowsMessageHandler.h"
#include "System.h"
#include "WindowsWindow.h"

#define ASSERT_HRES_MESSAGE(hResult, message) \
	if (FAILED(hResult)) \
	{ \
		LOG_ERROR(WindowsSystem::GetLastSystemError() + " " + message); \
		STOP_EXECUTION; \
	} 

#define ASSERT_HRES(hResult) \
	ASSERT_HRES_MESSAGE(hResult, "#cond asserted to false!")

namespace Rhygine
{
	class Config;

	class WindowsSystem : public System
	{
	public:
		WindowsSystem(HINSTANCE t_instance, HINSTANCE t_prev_instance, Config& t_config);
		WindowsSystem() = delete;
		~WindowsSystem();

		[[nodiscard]] WindowsSystem* GetInstance();

		std::optional<int> ProcessMessages() override;

		bool CreateConsole() override;
		bool DestroyConsole() override;

		Window::WindowId AddWindow() override;
		[[nodiscard]] virtual Window* GetWindow(Window::WindowId t_id) const override;
		bool DestroyWindow(Window::WindowId t_id) override;

		bool AddMessageHandler(IWindowsMessageHandler* t_message_handler);
		bool RemoveMessageHandler(IWindowsMessageHandler* t_message_handler);

		[[nodiscard]] static std::string GetLastSystemError();

	private:
		static LRESULT CALLBACK ProcessPassthrough(HWND t_window, UINT t_message, WPARAM t_w_param, LPARAM t_l_param);
		LRESULT ProcessMessage(HWND t_window, UINT t_message, WPARAM t_w_param, LPARAM t_l_param);
		
	private:
		HINSTANCE m_instance;
		HINSTANCE m_prev_instance;
		std::string m_window_class_name = "Rhygine Window";
		std::vector<IWindowsMessageHandler*> m_message_handlers;
		std::vector<WindowsWindow*> m_windows;

		static WindowsSystem* s_instance;
	};
}
