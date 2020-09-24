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

}

DirectX::XMMATRIX TestCamera::GetMatrix()
{
	DirectX::XMVECTOR upVector = DirectX::XMVector3Transform(
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
		DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z)
	);

	return DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(position.x, position.y, position.z, 0.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		upVector
	);

	//	return DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
	//		DirectX::XMMatrixTranslation(position.x, position.y, position.z);
}
