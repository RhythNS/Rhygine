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
		DirectX::XMMatrixTranslation(-transform->position.m_floats[0], -transform->position.m_floats[1], -transform->position.m_floats[2]) *
		DirectX::XMMatrixRotationQuaternion(
			DirectX::XMVectorSet(-transform->rotation.getX(), -transform->rotation.getY(), -transform->rotation.getZ(), transform->rotation.getW())
		);
}
