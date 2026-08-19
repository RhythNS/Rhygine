#pragma once

#include <optional>
#include <memory>

#include "Window.h"
#include "File/FileManager.h"

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
		[[nodiscard]] virtual Window* GetWindow(Window::WindowId t_id) const = 0;
		virtual bool DestroyWindow(Window::WindowId t_id) = 0;

		[[nodiscard]] FileManager& GetFileManager() const;

	private:
		std::unique_ptr<FileManager> m_fileManager;
		static System* s_instance;
	};
}