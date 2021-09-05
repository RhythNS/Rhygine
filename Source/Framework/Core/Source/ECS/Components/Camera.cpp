#include <ECS/Components/Camera.h>
#include <ECS/GameObject.h>
#include <ECS/Components/Transform.h>
#include <RhyMath.h>

void Camera::Init()
{
	transform = GetGameObject()->AddComponent<Transform>();
	assert(transform);
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
	// First move and then rotate
	matrix =
		DirectX::XMMatrixTranslation(-transform->localPosition.x, -transform->localPosition.y, -transform->localPosition.z) *
		DirectX::XMMatrixRotationQuaternion(
			DirectX::XMVectorSet(-transform->localRotation.x, -transform->localRotation.y, -transform->localRotation.z, transform->localRotation.w)
		);
}
