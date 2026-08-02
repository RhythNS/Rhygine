#include "WindowsWindow.h"

Rhygine::WindowsWindow::WindowsWindow(WindowId t_id, bool t_primary, uint32_t t_width, uint32_t t_height, uint32_t t_pos_x, uint32_t t_pos_y, HWND t_handle)
	: Window(t_primary, t_id, t_width, t_height, t_pos_x, t_pos_y), m_handle(t_handle)
{
}

void Rhygine::WindowsWindow::Resize(uint32_t t_pos_x, uint32_t t_pos_y, uint32_t t_width, uint32_t t_height)
{
}

bool Rhygine::WindowsWindow::SetTitle(const std::string& m_title)
{
	return false;
}

bool Rhygine::WindowsWindow::SetCaptureMouse(bool m_enable)
{
	return false;
}

bool Rhygine::WindowsWindow::SetFullscreenMode(FullscreenMode m_mode)
{
	return false;
}

HWND Rhygine::WindowsWindow::GetHandle() const
{
	return m_handle;
}
