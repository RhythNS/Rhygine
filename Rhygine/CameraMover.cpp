#include "CameraMover.h"
#include "Transform.h"
#include "Keys.h"
#include "Mouse.h"
#include "GameObject.h"
#include "Rhyimgui.h"

#include <string>

void CameraMover::Init()
{
	transform = GetGameObject()->GetComponent<Transform>();
}

void CameraMover::Update()
{
	Keys* keys = GetKeys();
	Mouse* mouse = GetMouse();
	float delta = GetDelta();

	// Rotation control with ZHGJTU
	RhyM::Vec3 rotate = RhyM::Vec3(0.0f, 0.0f, 0.0f);
	if (keys->IsKeyDown('Z'))
		rotate.m_floats[2] -= 5 * delta;
	if (keys->IsKeyDown('H'))
		rotate.m_floats[2] += 5 * delta;
	if (keys->IsKeyDown('G'))
		rotate.m_floats[0] -= 5 * delta;
	if (keys->IsKeyDown('J'))
		rotate.m_floats[0] += 5 * delta;
	if (keys->IsKeyDown('T'))
		rotate.m_floats[1] += 5 * delta;
	if (keys->IsKeyDown('U'))
		rotate.m_floats[1] -= 5 * delta;

	// Rotate the transform
	//transform->rotation = transform->rotation * RhyM::Quat(rotate.x(), rotate.y(), rotate.z());

	static float speed = 0.005f;
	// Get the relative position of the mouse.
	RhyM::Vec2I* rp = mouse->GetRelativePosition();

	/*
	// Display a imgui window for changing some values of the camera.
	ImGui::Begin("Mouse relative pos");
	ImGui::DragFloat("MouseSpeed", &speed, 0.01f, 0.01f, 1.0f);
	std::string rps = "Vec: " + std::to_string(rp->x) + " " + std::to_string(rp->y);
	ImGui::Text(rps.c_str());
	ImGui::End();
	*/

	// Rotate the camera if the right mouse button was pressed with mouse control.
	if (mouse->IsButtonDown(RH_MOUSE_RIGHT))
	{
		transform->localRotation =
			RhyM::Quat(RhyM::Vec3(0.0f, 1.0f, 0.0f), rp->x * speed) *
			transform->localRotation *
			RhyM::Quat(RhyM::Vec3(1.0f, 0.0f, 0.0f), rp->y * speed)
			;
	}

	// Move the camera around with WASDQE
	RhyM::Vec3 move = RhyM::Vec3(0.0f, 0.0f, 0.0f);
	if (keys->IsKeyDown('S'))
		move.m_floats[2] -= 5 * delta;
	if (keys->IsKeyDown('W'))
		move.m_floats[2] += 5 * delta;
	if (keys->IsKeyDown('A'))
		move.m_floats[0] -= 5 * delta;
	if (keys->IsKeyDown('D'))
		move.m_floats[0] += 5 * delta;
	if (keys->IsKeyDown('Q'))
		move.m_floats[1] -= 5 * delta;
	if (keys->IsKeyDown('E'))
		move.m_floats[1] += 5 * delta;

	// To make the camera feel more natrual, the move is rotated based on the current rotation of the camera.
	// So the movement is now in local space and not in global space.
	move = quatRotate(transform->localRotation, move);
	transform->localPosition += move;

	// If the r keys is pressed, then reset the rotation.
	if (keys->IsKeyDown('R'))
		transform->localRotation = RhyM::Quat(0.0f, 0.0f, 0.0f);
}
