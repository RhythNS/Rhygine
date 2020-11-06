#include "AwakeRigidbodies.h"
#include "Keys.h"
#include "RigidBody.h"

void AwakeRigidbodies::Set(std::vector<RigidBody*> _bodies)
{
	bodies = _bodies;
}

void AwakeRigidbodies::Update()
{
	if (GetKeys()->IsKeyDown('V'))
	{
		for (auto& body : bodies)
		{
			body->GetBody()->activate();
		}
	}
}
