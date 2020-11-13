#include "Transform.h"
#include "GameObject.h"
#include "Stage.h"
#include "Camera.h"
#include "Window.h"

Transform::~Transform()
{
	for (auto& child : children)
		GetGameObject()->GetStage()->RemoveGameObject(child->GetGameObject());
}

void Transform::SetParent(Transform* trans)
{
	if (parent == trans)
		return;

	RhyM::Vec3 oldPosition = GetWorldPosition();

	// If the old parent is not null then remove the reference to this transfrom.
	if (parent != nullptr)
		std::erase(parent->children, this);

	// If the new parent is not null then add it a reference to it.
	if (trans != nullptr)
		trans->children.push_back(this);

	// Set the local position so the acctual position does not change.
	SetWorldPosition(oldPosition);

	// Assign the parent to the new value
	parent = trans;
}

Transform* Transform::GetParent()
{
	return parent;
}

Transform* Transform::GetChildAt(int index)
{
	assert(index > -1 && index < children.size());
	return children[index];
}

int Transform::GetChildCount()
{
	return children.size();
}

RhyM::Vec3 Transform::GetForward()
{
	return quatRotate(GetWorldRotation(), RhyM::Vec3(0.0f, 0.0f, 1.0f));
}

RhyM::Vec3 Transform::GetBackwards()
{
	return quatRotate(GetWorldRotation(), RhyM::Vec3(0.0f, 0.0f, -1.0f));
}

RhyM::Vec3 Transform::GetLeft()
{
	return quatRotate(GetWorldRotation(), RhyM::Vec3(-1.0f, 0.0f, 0.0f));
}

RhyM::Vec3 Transform::GetRight()
{
	return quatRotate(GetWorldRotation(), RhyM::Vec3(1.0f, 0.0f, 0.0f));
}

RhyM::Vec3 Transform::GetUp()
{
	return quatRotate(GetWorldRotation(), RhyM::Vec3(0.0f, 1.0f, 0.0f));
}

RhyM::Vec3 Transform::GetDown()
{
	return quatRotate(GetWorldRotation(), RhyM::Vec3(0.0f, -1.0f, 0.0f));
}

DirectX::XMMATRIX Transform::GetWorldMatrix()
{
	// Scale the object ->
	// Rotate the object ->
	// Move the object
	if (parent == nullptr)
		return
		DirectX::XMMatrixScaling(localScale.m_floats[0], localScale.m_floats[1], localScale.m_floats[2]) *
		DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(localRotation.getX(), localRotation.getY(), localRotation.getZ(), localRotation.getW())) *
		DirectX::XMMatrixTranslation(localPosition.m_floats[0], localPosition.m_floats[1], localPosition.m_floats[2])
		;
	// Scale the object ->
	// Rotate the object ->
	// Move the object ->
	// Scale, Rotate and move to parent
	else
		return
		DirectX::XMMatrixScaling(localScale.m_floats[0], localScale.m_floats[1], localScale.m_floats[2]) *
		DirectX::XMMatrixTranslation(localPosition.m_floats[0], localPosition.m_floats[1], localPosition.m_floats[2]) *
		DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(localRotation.getX(), localRotation.getY(), localRotation.getZ(), localRotation.getW())) *
		parent->GetWorldMatrix()
		;
}

DirectX::XMMATRIX Transform::GetPerspectiveMatrix()
{
	// Scale the object ->
	// Rotate the object ->
	// Move the object ->
	// Apply camera movement and rotation ->
	// Perspective matrix ->
	// Transpose to directX
	return
		DirectX::XMMatrixTranspose
		(
			GetWorldMatrix() *
			*GetGameObject()->GetStage()->Get3DCamera()->GetMatrix() *
			DirectX::XMMatrixPerspectiveLH(1.0f, (float)Window::GetInstance()->GetHeight() / (float)Window::GetInstance()->GetWidth(), 0.5f, 100.0f)
		);
}

DirectX::XMMATRIX Transform::GetLocalMatrix()
{
	RhyM::Vec3 scale = GetWorldScale();
	RhyM::Quat rotation = GetWorldRotation();

	// Scale the object ->
	// Rotate the object ->
	// Transpose to directX
	return
		DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixScaling(scale.m_floats[0], scale.m_floats[1], scale.m_floats[2]) *
			DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(rotation.getX(), rotation.getY(), rotation.getZ(), rotation.getW()))
		);
}

void Transform::SetWorldPosition(RhyM::Vec3 pos)
{
	localPosition = (parent == nullptr ? pos : pos - parent->GetWorldPosition());
}

RhyM::Vec3 Transform::GetWorldPosition()
{
	return parent == nullptr ? localPosition : localPosition + parent->GetWorldPosition();
}

void Transform::SetWorldRotation(RhyM::Quat rot)
{
	localRotation = (parent == nullptr ? rot : parent->GetWorldRotation() * -rot);
}

RhyM::Quat Transform::GetWorldRotation()
{
	return parent == nullptr ? localRotation : localRotation * parent->GetWorldRotation();
}

void Transform::SetWorldScale(RhyM::Vec3 scl)
{
	assert(scl.x() != 0 && scl.y() != 0 && scl.z() != 0);
	localScale = (parent == nullptr ? scl : scl / parent->GetWorldScale());
}

RhyM::Vec3 Transform::GetWorldScale()
{
	return parent == nullptr ? localScale : localScale * parent->GetWorldScale();
}
