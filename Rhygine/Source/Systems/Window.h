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

		[[nodiscard]] virtual int GetX() const;
		[[nodiscard]] virtual int GetY() const;

		[[nodiscard]] virtual int GetWidth() const;
		[[nodiscard]] virtual int GetHeight() const;

		[[nodiscard]] virtual bool IsPrimary() const;

		virtual void Resize(int t_pos_x, int t_pos_y, int t_width, int t_height) = 0;

		virtual bool SetTitle(const std::string& m_title) = 0;
		[[nodiscard]] virtual const std::string& GetTitle() const;

		virtual bool SetCaptureMouse(bool m_enable) = 0;
		[[nodiscard]] virtual bool IsCapturingMouse() const;

		virtual bool SetFullscreenMode(FullscreenMode m_mode) = 0;
		[[nodiscard]] virtual FullscreenMode GetFullscreenMode() const;

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
