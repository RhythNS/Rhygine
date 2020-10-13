#include "Transform.h"
#include "Window.h"
#include "Gfx.h"

void Transform::Init()
{
}

DirectX::XMMATRIX* Transform::GetPerspectiveMatrix()
{
	perspectiveMatrix =
		DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
			DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(rotation.x, rotation.y, rotation.z, rotation.w)) *
			DirectX::XMMatrixTranslation(position.x, position.y, position.z) *
			Window::GetInstance()->GetGfx()->camera.GetMatrix() *
			DirectX::XMMatrixPerspectiveLH(1.0f, (float)Window::GetInstance()->GetHeight() / (float)Window::GetInstance()->GetWidth(), 0.5f, 100.0f)
		);

	return &perspectiveMatrix;
}

DirectX::XMMATRIX* Transform::GetLocalMatrix()
{
	localMatrix =
		DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
			DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(rotation.x, rotation.y, rotation.z, rotation.w))
		);

	return &localMatrix;
}

DirectX::XMMATRIX* Transform::GetWorldMatrix()
{
	worldMatrix =
		DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
			DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(rotation.x, rotation.y, rotation.z, rotation.w)) *
			DirectX::XMMatrixTranslation(position.x, position.y, position.z)
		);

	return &worldMatrix;
}
