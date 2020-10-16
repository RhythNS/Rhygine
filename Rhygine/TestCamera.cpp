#include "TestCamera.h"
#include "Window.h"

TestCamera::TestCamera()
{
	position = { 0.0f, 0.0f, 0.0f };
	rotation = { 0.0f, 0.0f, 0.0f };
}

TestCamera::TestCamera(Vector position, Vector rotation) : position(position), rotation(rotation)
{
}

void TestCamera::HandleInput(Window* window)
{
	float delta = window->time.GetDelta();
	if (window->keys.IsKeyDown('S'))
		position.z += 5 * delta;
	if (window->keys.IsKeyDown('W'))
		position.z -= 5 * delta;
	if (window->keys.IsKeyDown('A'))
		position.x += 5 * delta;
	if (window->keys.IsKeyDown('D'))
		position.x -= 5 * delta;
	if (window->keys.IsKeyDown('Q'))
		position.y += 5 * delta;
	if (window->keys.IsKeyDown('E'))
		position.y -= 5 * delta;

	if (window->keys.IsKeyDown('H'))
		rotation.z += 5 * delta;
	if (window->keys.IsKeyDown('Z'))
		rotation.z -= 5 * delta;
	if (window->keys.IsKeyDown('G'))
		rotation.x += 5 * delta;
	if (window->keys.IsKeyDown('J'))
		rotation.x -= 5 * delta;
	if (window->keys.IsKeyDown('T'))
		rotation.y += 5 * delta;
	if (window->keys.IsKeyDown('U'))
		rotation.y -= 5 * delta;

	if (window->keys.IsKeyDownThisFrame(VK_SPACE))
	{
		rotation = { 0,0,0 };
		position = { 0,0,0 };
	}
}

DirectX::XMMATRIX TestCamera::GetMatrix()
{
	return
		DirectX::XMMatrixTranslation(position.x, position.y, position.z) *
		DirectX::XMMatrixRotationRollPitchYaw(rotation.x, -rotation.y, rotation.z);
}
