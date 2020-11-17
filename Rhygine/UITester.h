#pragma once
#include "Component.h"
#include "Texture.h"
#include "Updatable.h"
#include "MonoFont.h"

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
