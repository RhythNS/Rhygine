#pragma once

#include <optional>

#include "Window.h"

namespace Rhygine
{
	class Config;

	class System
	{
	public:
		System() = delete;

		System(Config& t_config);
		virtual ~System();

		[[nodiscard]] static System* GetInstance();

		[[nodiscard]] virtual std::optional<int> ProcessMessages() = 0;

		virtual bool CreateConsole() = 0;
		virtual bool DestroyConsole() = 0;

		virtual Window::WindowId AddWindow() = 0;
		virtual bool DestroyWindow(Window::WindowId t_id) = 0;

	private:
		static System* s_instance;
	};
}