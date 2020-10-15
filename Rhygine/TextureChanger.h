#pragma once
#include "Bindable.h"
#include "Updatable.h"

#include <memory>
#include <vector>

class Texture;

class TextureChanger : public Bindable, public Updatable
{
public:
	void Init();
	void Bind();
	void Update();
private:
	int atTexture = 0;
	float timeToChange = 1.0f / 30.0f;
	float timer = timeToChange;
	std::vector<std::unique_ptr<Texture>> textures;
};
