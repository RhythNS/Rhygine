#pragma once
#include <ECS/Component.h>
#include <ECS/Updatable.h>

class Transform;

/// <summary>
/// Makes the camera controllable with the keyboard and mouse.
/// </summary>
class CameraMover : public Component, public Updatable
{
public:
	void Init();
	void Update();
private:
	Transform* transform;
};
