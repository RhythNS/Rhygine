#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include "RhyMath.h"

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
		DirectX::XMMatrixTranslation(-transform->localPosition.m_floats[0], -transform->localPosition.m_floats[1], -transform->localPosition.m_floats[2]) *
		DirectX::XMMatrixRotationQuaternion(
			DirectX::XMVectorSet(-transform->localRotation.getX(), -transform->localRotation.getY(), -transform->localRotation.getZ(), transform->localRotation.getW())
		);
}
