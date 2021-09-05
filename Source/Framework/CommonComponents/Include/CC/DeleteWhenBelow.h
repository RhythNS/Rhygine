#pragma once
#include <ECS/Component.h>
#include <ECS/Updatable.h>
#include <ECS/Components/Transform.h>

/// <summary>
/// Deletes a gameobject when it is below a given value.
/// </summary>
class DeleteWhenBelow : public Component, public Updatable
{
public:
	void Init();
	void Update();
	float y = -10;

private:
	Transform* trans = nullptr;
};
