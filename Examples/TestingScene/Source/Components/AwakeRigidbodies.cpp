#include <Components/AwakeRigidbodies.h>
#include <Input/Keys.h>
#include <Bullet/RigidBody.h>

void AwakeRigidbodies::Set(std::vector<RhyBullet::RigidBody*> _bodies)
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
