#include "Gameobject.h"
#include "Window.h"
#include "Gfx.h"

#include <memory>

void Gameobject::Init()
{
}

void Gameobject::Update()
{
	consBuffer->SetAndUpdate(*transform.GetPerspectiveMatrix());
}

void Gameobject::Draw()
{
	for (auto& bindable : bindables)
		bindable->Bind();

	Window::GetInstance()->GetGfx()->DrawIndexed(indexBuffer->GetSize());
}

void Gameobject::CreateTransform()
{
	Transform::TransformBuffer matConsBuffer = *transform.GetPerspectiveMatrix();
	bindables.push_back(std::make_unique<ConstantVS<Transform::TransformBuffer>>(matConsBuffer, 0));
	consBuffer = static_cast<ConstantVS<Transform::TransformBuffer>*>(bindables[bindables.size() - 1].get());
}
