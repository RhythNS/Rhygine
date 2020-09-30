#pragma once
#include <vector>
#include <memory>

#include "Gameobject.h"
#include "Texture.h"
#include "RhyWin.h"

class TestTexture : public Gameobject
{
public:
	virtual void Init();
	virtual void Update();
	virtual void Draw();
private:
	int atTexture = 0;
	float timeToChange = 1.0f / 30.0f;
	float timer = timeToChange;
	std::vector<std::unique_ptr<Texture>> textures;
};
