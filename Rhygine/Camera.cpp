#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include "RhyMath.h"

void Camera::Init()
{
	transform = GetGameObject()->AddComponent<Transform>();
}

Transform* Camera::GetTransform()
{
	return transform;
}

DirectX::XMMATRIX* Camera::GetMatrix()
{
	return &matrix;
}

void Camera::UpdateMatrix()
{
	matrix =
		DirectX::XMMatrixTranslation(-transform->position.x, -transform->position.y, -transform->position.z) *
		DirectX::XMMatrixRotationQuaternion(
			DirectX::XMVectorSet(-transform->rotation.x, -transform->rotation.y, -transform->rotation.z, transform->rotation.w)
		);
}
