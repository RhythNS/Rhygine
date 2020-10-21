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

	RhyM::Vec3 rotate;
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

	transform->rotation = transform->rotation * RhyM::Quat(rotate.x, rotate.y, rotate.z);

	static float speed = 0.01f;
	RhyM::Vec2I* rp = mouse->GetRelativePosition();
	ImGui::Begin("Mouse relative pos");
	ImGui::DragFloat("MouseSpeed", &speed, 0.01f, 0.01f, 1.0f);
	std::string rps = "Vec: " + std::to_string(rp->x) + " " + std::to_string(rp->y);
	ImGui::Text(rps.c_str());
	ImGui::End();


	if (mouse->IsButtonDown(RH_MOUSE_RIGHT))
	{
		transform->rotation = transform->rotation * RhyM::Quat(rp->x * speed, 0.0f, rp->y * speed);
	}


	RhyM::Vec3 move;
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

	move = transform->rotation.Rotate(move);

	ImGui::Begin("CameraMover");
	std::string str = "Vec: " + std::to_string(move.x) + " " + std::to_string(move.y) + " " + std::to_string(move.z);
	ImGui::Text(str.c_str());
	transform->position += move;

	move = transform->position;
	str = "At: " + std::to_string(move.x) + " " + std::to_string(move.y) + " " + std::to_string(move.z);
	ImGui::Text(str.c_str());
	ImGui::End();

	if (keys->IsKeyDown('R'))
		transform->rotation = RhyM::Quat();
}