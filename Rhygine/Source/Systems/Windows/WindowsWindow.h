#pragma once

#include "Window.h"

#include "Windows/RhyWindows.h"

namespace Rhygine
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow() = delete;
		WindowsWindow(WindowId t_id, bool t_primary, uint32_t t_width, uint32_t t_height, uint32_t t_pos_x, uint32_t t_pos_y, HWND t_handle);
		~WindowsWindow() = default;

		virtual void Resize(uint32_t t_pos_x, uint32_t t_pos_y, uint32_t t_width, uint32_t t_height) override;

		virtual bool SetTitle(const std::string& m_title) override;
		
		virtual bool SetCaptureMouse(bool m_enable) override;

		virtual bool SetFullscreenMode(FullscreenMode m_mode) override;

		HWND GetHandle() const;

		[[nodiscard]] virtual void* GetNativeHandle() const override { return m_handle; }

	private:
		HWND m_handle;
	};
}
