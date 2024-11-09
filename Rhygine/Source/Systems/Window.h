#pragma once

#include <string>

namespace Rhygine
{
	class Window
	{
	public:
		using WindowId = unsigned int;

		enum class FullscreenMode
		{
			Windowed, Borderless, Exclusive
		};

		Window() = delete;
		Window(WindowId t_id, bool t_primary, int t_width, int t_height, int t_pos_x, int t_pos_y);
		virtual ~Window() = default;

		virtual [[nodiscard]] int GetX() const;
		virtual [[nodiscard]] int GetY() const;

		virtual [[nodiscard]] int GetWidth() const;
		virtual [[nodiscard]] int GetHeight() const;

		virtual [[nodiscard]] bool IsPrimary() const;

		virtual void Resize(int t_pos_x, int t_pos_y, int t_width, int t_height) = 0;

		virtual bool SetTitle(const std::string& m_title) = 0;
		virtual [[nodiscard]] const std::string& GetTitle() const;

		virtual bool SetCaptureMouse(bool m_enable) = 0;
		virtual [[nodiscard]] bool IsCapturingMouse() const;

		virtual bool SetFullscreenMode(FullscreenMode m_mode) = 0;
		virtual [[nodiscard]] FullscreenMode GetFullscreenMode() const;

		WindowId GetID();

	protected:
		int m_width = 0;
		int m_height = 0;
		int m_pos_x = 0;
		int m_pos_y = 0;
		FullscreenMode m_mode = FullscreenMode::Windowed;
		bool m_capture_mouse = false;
		std::string m_title = "Rhygine";

	private:
		WindowId m_id;
		bool m_primary = false;
	};
}
