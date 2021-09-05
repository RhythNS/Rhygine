#include <ECS/Components/CameraMover.h>
#include <ECS/Components/Transform.h>
#include <Input/Keys.h>
#include <Input/Mouse.h>
#include <ECS/GameObject.h>

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
		rotate.z -= 5 * delta;
	if (keys->IsKeyDown('H'))
		rotate.z += 5 * delta;
	if (keys->IsKeyDown('G'))
		rotate.x -= 5 * delta;
	if (keys->IsKeyDown('J'))
		rotate.x += 5 * delta;
	if (keys->IsKeyDown('T'))
		rotate.y += 5 * delta;
	if (keys->IsKeyDown('U'))
		rotate.y -= 5 * delta;

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
			RhyM::Quat::FromAngleAxis(rp->x * speed, RhyM::Vec3(0.0f, 1.0f, 0.0f)) *
			transform->localRotation *
			RhyM::Quat::FromAngleAxis(rp->y * speed, RhyM::Vec3(1.0f, 0.0f, 0.0f))
			;
	}

	// Move the camera around with WASDQE
	RhyM::Vec3 move = RhyM::Vec3(0.0f, 0.0f, 0.0f);
	if (keys->IsKeyDown('S'))
		move.z -= 5 * delta;
	if (keys->IsKeyDown('W'))
		move.z += 5 * delta;
	if (keys->IsKeyDown('A'))
		move.x -= 5 * delta;
	if (keys->IsKeyDown('D'))
		move.x += 5 * delta;
	if (keys->IsKeyDown('Q'))
		move.y -= 5 * delta;
	if (keys->IsKeyDown('E'))
		move.y += 5 * delta;

	// To make the camera feel more natrual, the move is rotated based on the current rotation of the camera.
	// So the movement is now in local space and not in global space.
	move = transform->localRotation * move;
	transform->localPosition += move;

	// If the r keys is pressed, then reset the rotation.
	if (keys->IsKeyDown('R'))
		transform->localRotation = RhyM::Quat::FromEuler(0.0f, 0.0f, 0.0f);
}
