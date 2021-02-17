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
		body->GetBody()->applyForce(btVector3(0.0f, 20.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
	if (GetKeys()->IsKeyDown('Y'))
		body->GetBody()->applyImpulse(btVector3(1.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
	if (GetKeys()->IsKeyDown('C'))
		body->GetBody()->applyImpulse(btVector3(-1.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
	else
		return;
	body->GetBody()->activate(true);
}
