#pragma once
#include "Component.h"
#include "Updatable.h"

#include <memory>
#include <vector>

class Texture;

class TextureChanger : public Component, public Updatable
{
public:
	void Init();
	void Update();
private:
	int atTexture = 0;
	float timeToChange = 1.0f / 30.0f;
	float timer = timeToChange;
	std::vector<std::unique_ptr<Texture>> textures;
};

