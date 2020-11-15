#pragma once
#include "Component.h"
#include "Texture.h"
#include "Updatable.h"
#include "MonoFont.h"

#include <memory>

class UITester : public Component, public Updatable
{
public:
	void Init();
	void Update();
private:
	std::unique_ptr<Texture> orange, white, blue, black, silver, sprite;
	std::unique_ptr<MonoFont> font;
};
