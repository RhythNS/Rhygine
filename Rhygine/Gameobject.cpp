#include "Gameobject.h"
#include "Window.h"
#include "Gfx.h"

void Gameobject::Init()
{
}

void Gameobject::Update()
{
	consBuffer->SetAndUpdate(GetCurrentTransform());
}

void Gameobject::Draw()
{
	for (auto& bindable : bindables)
		bindable->Bind();

	Window::GetInstance()->GetGfx()->DrawIndexed(indexBuffer->GetSize());
}

Gameobject::MatrixConstantBuffer Gameobject::GetCurrentTransform()
{
	return {
		DirectX::XMMatrixTranspose(
			DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
			DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) *
			DirectX::XMMatrixTranslation(position.x, position.y, position.z) *
			Window::GetInstance()->GetGfx()->camera.GetMatrix() *
			DirectX::XMMatrixPerspectiveLH(1.0f, (float)Window::GetInstance()->GetHeight() / (float)Window::GetInstance()->GetWidth(), 0.5f, 100.0f)
			)
	};
}
