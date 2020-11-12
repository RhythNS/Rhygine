#include "OrthographicCamera.h"
#include "Window.h"
#include "Gfx.h"

OrthographicCamera::OrthographicCamera(RhyM::Vec3 position, RhyM::Vec2 rotation, RhyM::Vec2 zoom)
	: position(position), rotation(rotation), zoom(zoom)
{ }

DirectX::XMMATRIX OrthographicCamera::GetOrthoMatrix()
{
	Window* win = Window::GetInstance();
	return DirectX::XMMatrixTranspose
	(
		DirectX::XMMatrixTranslation(position.m_floats[0], position.m_floats[1], position.m_floats[2]) *
		//DirectX::XMMatrixRotationX(rotation.x) *
		//DirectX::XMMatrixRotationY(rotation.y) *
		DirectX::XMMatrixOrthographicLH(((float)win->GetWidth()) * zoom.x, ((float)win->GetHeight()) * zoom.y, win->GetGfx()->nearZ, win->GetGfx()->farZ)
	);
}
