#include "Transform.h"
#include "GameObject.h"
#include "Stage.h"
#include "Camera.h"
#include "Window.h"

void Transform::Init()
{
}

DirectX::XMMATRIX* Transform::GetPerspectiveMatrix()
{
	// Scale the object ->
	// Rotate the object ->
	// Move the object ->
	// Apply camera movement and rotation ->
	// Perspective matrix ->
	// Transpose to directX
	perspectiveMatrix =
		DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixScaling(scale.m_floats[0], scale.m_floats[1], scale.m_floats[2]) *
			DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(rotation.getX(), rotation.getY(), rotation.getZ(), rotation.getW())) *
			DirectX::XMMatrixTranslation(position.m_floats[0], position.m_floats[1], position.m_floats[2]) *
			*GetGameObject()->GetStage()->GetCamera()->GetMatrix() *
			DirectX::XMMatrixPerspectiveLH(1.0f, (float)Window::GetInstance()->GetHeight() / (float)Window::GetInstance()->GetWidth(), 0.5f, 100.0f)
		);

	return &perspectiveMatrix;
}

DirectX::XMMATRIX* Transform::GetLocalMatrix()
{
	// Scale the object ->
	// Rotate the object ->
	// Transpose to directX
	localMatrix =
		DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixScaling(scale.m_floats[0], scale.m_floats[1], scale.m_floats[2]) *
			DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(rotation.getX(), rotation.getY(), rotation.getZ(), rotation.getW()))
		);

	return &localMatrix;
}

RhyM::Vec3 Transform::GetForward()
{
	return quatRotate(rotation, RhyM::Vec3(0.0f, 0.0f, 1.0f));
}

DirectX::XMMATRIX* Transform::GetWorldMatrix()
{	
	// Scale the object ->
	// Rotate the object ->
	// Move the object ->
	// tTranspose to directX
	worldMatrix =
		DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixScaling(scale.m_floats[0], scale.m_floats[1], scale.m_floats[2]) *
			DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(rotation.getX(), rotation.getY(), rotation.getZ(), rotation.getW())) *
			DirectX::XMMatrixTranslation(position.m_floats[0], position.m_floats[1], position.m_floats[2])
		);

	return &worldMatrix;
}
