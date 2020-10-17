#include "CameraMover.h"
#include "Transform.h"
#include "Keys.h"
#include "GameObject.h"

void CameraMover::Init()
{
	transform = GetGameObject()->GetComponent<Transform>();
}

void CameraMover::Update()
{
	Keys* keys = GetKeys();
	float delta = GetDelta();

	RhyM::Vec3 rotate;
	if (keys->IsKeyDown('Z'))
		rotate.z += 5 * delta;
	if (keys->IsKeyDown('H'))
		rotate.z -= 5 * delta;
	if (keys->IsKeyDown('G'))
		rotate.x += 5 * delta;
	if (keys->IsKeyDown('J'))
		rotate.x -= 5 * delta;
	if (keys->IsKeyDown('T'))
		rotate.y += 5 * delta;
	if (keys->IsKeyDown('U'))
		rotate.y -= 5 * delta;

	transform->rotation = transform->rotation * RhyM::Quat(rotate.x, rotate.y, rotate.z);

	RhyM::Vec3 move;
	if (keys->IsKeyDown('S'))
		move.z += 5 * delta;
	if (keys->IsKeyDown('W'))
		move.z -= 5 * delta;
	if (keys->IsKeyDown('A'))
		move.x += 5 * delta;
	if (keys->IsKeyDown('D'))
		move.x -= 5 * delta;
	if (keys->IsKeyDown('Q'))
		move.y += 5 * delta;
	if (keys->IsKeyDown('E'))
		move.y -= 5 * delta;

	transform->position += transform->rotation.Rotate(move);
}
