#include "Window.h"

Rhygine::Window::Window(WindowId t_id, bool t_primary, int t_width, int t_height, int t_pos_x, int t_pos_y)
	: m_primary(t_primary), m_id(t_id), m_width(t_width), m_height(t_height), m_pos_x(t_pos_x), m_pos_y(t_pos_y)
{

}

int Rhygine::Window::GetX() const
{
	return m_pos_x;
}

int Rhygine::Window::GetY() const
{
	return m_pos_y;
}

int Rhygine::Window::GetWidth() const
{
	return m_width;
}

int Rhygine::Window::GetHeight() const
{
	return m_height;
}

bool Rhygine::Window::IsPrimary() const
{
	return m_primary;
}

const std::string& Rhygine::Window::GetTitle() const
{
	return m_title;
}

bool Rhygine::Window::IsCapturingMouse() const
{
	return m_capture_mouse;
}

Rhygine::Window::FullscreenMode Rhygine::Window::GetFullscreenMode() const
{
	return m_mode;
}

Rhygine::Window::WindowId Rhygine::Window::GetID()
{
	return m_id;
}
