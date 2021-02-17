#include "OrthographicCamera.h"
#include "Window.h"
#include "Gfx.h"

OrthographicCamera::OrthographicCamera(RhyM::Vec3 position, RhyM::Vec2 rotation, RhyM::Vec2 zoom)
	: position(position), rotation(rotation), zoom(zoom)
{ }

DirectX::XMMATRIX OrthographicCamera::GetOrthoMatrix()
{
	Window* win = Window::GetInstance();
	// Move the object ->
	// Rotate the object ->
	// Apply orthographic transformation ->
	// Transpose the matirx
	return DirectX::XMMatrixTranspose
	(
		DirectX::XMMatrixTranslation(position.x, position.y, position.z) *
		DirectX::XMMatrixRotationX(rotation.x) *
		DirectX::XMMatrixRotationY(rotation.y) *
		DirectX::XMMatrixOrthographicLH(((float)win->GetWidth()) * zoom.x, ((float)win->GetHeight()) * zoom.y, win->GetGfx()->nearZ, win->GetGfx()->farZ)
	);
}
