#pragma once
#include "Component.h"
#include "Updatable.h"
#include "Transform.h"
#include "RigidBody.h"

#include <functional>

class BulletShowcaser;
class DeleteWhenBelow : public Component, public Updatable
{
public:
	void Init();
	void Update();
	float y = -10;

	BulletShowcaser* bulletShowcaser;
private:
	Transform* trans;
};
