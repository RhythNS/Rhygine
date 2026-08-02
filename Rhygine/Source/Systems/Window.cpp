#include "Window.h"

Rhygine::Window::Window(WindowId t_id, bool t_primary, uint32_t t_width, uint32_t t_height, uint32_t t_pos_x, uint32_t t_pos_y)
	: m_primary(t_primary), m_id(t_id), m_width(t_width), m_height(t_height), m_pos_x(t_pos_x), m_pos_y(t_pos_y)
{

}

uint32_t Rhygine::Window::GetX() const
{
	return m_pos_x;
}

uint32_t Rhygine::Window::GetY() const
{
	return m_pos_y;
}

uint32_t Rhygine::Window::GetWidth() const
{
	return m_width;
}

uint32_t Rhygine::Window::GetHeight() const
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

Rhygine::Window::WindowId Rhygine::Window::GetID() const
{
	return m_id;
}
