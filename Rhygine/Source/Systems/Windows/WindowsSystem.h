#pragma once

#include <string>
#include <vector>

#include "Windows.h"
#include "IWindowsMessageHandler.h"
#include "System.h"

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
		bool DestroyWindow(Window::WindowId t_id) override;

		bool AddMessageHandler(IWindowsMessageHandler* t_message_handler);
		bool RemoveMessageHandler(IWindowsMessageHandler* t_message_handler);

		[[nodiscard]] std::string GetLastSystemError();

	private:
		static LRESULT CALLBACK ProcessPassthrough(HWND t_window, UINT t_message, WPARAM t_w_param, LPARAM t_l_param);
		LRESULT ProcessMessage(HWND t_window, UINT t_message, WPARAM t_w_param, LPARAM t_l_param);

	private:
		HINSTANCE m_instance;
		HINSTANCE m_prev_instance;
		std::string m_window_class_name = "Rhygine Window";
		std::vector<IWindowsMessageHandler*> m_message_handlers;

		static WindowsSystem* s_instance;
	};
}
