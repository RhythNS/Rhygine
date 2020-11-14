#pragma once
#include "Component.h"
#include "Texture.h"

#include <memory>

class UITester : public Component
{
public:
	void Init();
private:
	std::unique_ptr<Texture> orange, white, blue, black, silver, sprite;
};
