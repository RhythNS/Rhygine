#pragma once
#include <ECS/Component.h>
#include <ECS/Updatable.h>
#include <ECS/Bindable/Texture.h>
#include <2D/MonoFont.h>

#include <memory>

/// <summary>
/// Test for adding various ui components to the stage.
/// </summary>
class UITester : public Component, public Updatable
{
public:
	void Init();
	void Update();
private:
	std::unique_ptr<Texture> orange, white, blue, black, silver, sprite;
	std::unique_ptr<MonoFont> font;
};
