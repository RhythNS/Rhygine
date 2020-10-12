#include "Transform.h"
#include "Window.h"
#include "Gfx.h"

Transform::TransformBuffer* Transform::GetPerspectiveMatrix()
{
	perspectiveBuffer = 
	{
		DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
			DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(rotation.x, rotation.y, rotation.z, rotation.w)) *
			DirectX::XMMatrixTranslation(position.x, position.y, position.z) *
			Window::GetInstance()->GetGfx()->camera.GetMatrix() *
			DirectX::XMMatrixPerspectiveLH(1.0f, (float)Window::GetInstance()->GetHeight() / (float)Window::GetInstance()->GetWidth(), 0.5f, 100.0f)
		)
	};
	return &perspectiveBuffer;
}

Transform::TransformBuffer* Transform::GetLocalMatrix()
{
	localBuffer =
	{
		DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
			DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(rotation.x, rotation.y, rotation.z, rotation.w))
		)
	};

	return &localBuffer;
}

Transform::TransformBuffer* Transform::GetWorldMatrix()
{
	worldBuffer = 
	{
		DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
			DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(rotation.x, rotation.y, rotation.z, rotation.w)) *
			DirectX::XMMatrixTranslation(position.x, position.y, position.z)
		)
	};

	return &worldBuffer;
}
