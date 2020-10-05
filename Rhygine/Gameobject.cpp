#include "Gameobject.h"
#include "Window.h"
#include "Gfx.h"

#include <memory>

void Gameobject::Init()
{
}

void Gameobject::Update()
{
	consBuffer->SetAndUpdate(*transform.GetCurrentTransform());
}

void Gameobject::Draw()
{
	for (auto& bindable : bindables)
		bindable->Bind();

	Window::GetInstance()->GetGfx()->DrawIndexed(indexBuffer->GetSize());
}

void Gameobject::CreateTransform()
{
	Transform::TransformBuffer matConsBuffer = *transform.GetCurrentTransform();
	bindables.push_back(std::make_unique<ConstantVS<Transform::TransformBuffer>>(matConsBuffer, 0));
	consBuffer = static_cast<ConstantVS<Transform::TransformBuffer>*>(bindables[bindables.size() - 1].get());
}
