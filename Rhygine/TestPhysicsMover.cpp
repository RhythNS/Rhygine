#include "TestPhysicsMover.h"
#include "Keys.h"
#include "Gameobject.h"
#include "RhyMath.h"
#include "RigidBody.h"

void TestPhysicsMover::Init()
{
	body = GetGameObject()->GetComponent<RigidBody>();
}

void TestPhysicsMover::Update()
{
	if (GetKeys()->IsKeyDown('X'))
		body->GetBody()->applyImpulse(RhyM::Vec3(0.0f, 10.0f, 0.0f), RhyM::Vec3(0.0f, 0.0f, 0.0f));
	if (GetKeys()->IsKeyDown('Y'))
		body->GetBody()->applyImpulse(RhyM::Vec3(1.0f, 0.0f, 0.0f), RhyM::Vec3(0.0f, 0.0f, 0.0f));
	if (GetKeys()->IsKeyDown('C'))
		body->GetBody()->applyImpulse(RhyM::Vec3(-1.0f, 0.0f, 0.0f), RhyM::Vec3(0.0f, 0.0f, 0.0f));
	else
		return;
	body->GetBody()->activate(true);
}
