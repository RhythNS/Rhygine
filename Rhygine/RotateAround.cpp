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
	transform->localRotation = transform->localRotation * RhyM::Quat::FromEuler(delta * rotationSpeed.x, delta * rotationSpeed.y, delta * rotationSpeed.z);
}
