#pragma once

#include "Window.h"

#include "Windows/RhyWindows.h"

namespace Rhygine
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow() = delete;
		WindowsWindow(WindowId t_id, bool t_primary, int t_width, int t_height, int t_pos_x, int t_pos_y, HWND t_handle);
		~WindowsWindow() = default;

		virtual void Resize(int t_pos_x, int t_pos_y, int t_width, int t_height) override;

		virtual bool SetTitle(const std::string& m_title) override;
		
		virtual bool SetCaptureMouse(bool m_enable) override;

		virtual bool SetFullscreenMode(FullscreenMode m_mode) override;

		HWND GetHandle() const;

	private:
		HWND m_handle;
	};
}
