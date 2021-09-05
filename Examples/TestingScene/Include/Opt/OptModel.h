#pragma once
#include <ECS/Component.h>
#include <ECS/Bindable/Texture.h>

#include <memory>

/// <summary>
/// Tests to see the frame time of cubes with a lighting effect.
/// </summary>
class OptModel : public Component
{
public:
	void Init();
private:
	std::unique_ptr<Texture> tex;
};

