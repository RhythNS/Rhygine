#include "RotateAround.h"
#include "Transform.h"
#include "GameObject.h"
#include "Window.h"
#include "RhyMath.h"

void RotateAround::Init()
{
	transform = GetGameObject()->GetComponent<Transform>();
}

void RotateAround::Update()
{
	float delta = Window::GetInstance()->time.GetDelta();
	transform->rotation = transform->rotation * RhyM::Quat(delta * rotationSpeed.m_floats[0], delta * rotationSpeed.m_floats[1], delta * rotationSpeed.m_floats[2]);
}
