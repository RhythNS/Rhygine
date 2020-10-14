#include "TextureChanger.h"
#include "Texture.h"
#include "Window.h"

#include <string>

void TextureChanger::Init()
{
	for (int i = 1; i < 21; i++)
	{
		textures.push_back(std::make_unique<Texture>(("TestModels\\Sprite\\Walk (" + std::to_string(i) + ").png").c_str(), 0));
	}
}

void TextureChanger::Update()
{
	timer -= Window::GetInstance()->time.GetDelta();
	if (timer <= 0)
	{
		timer = timeToChange;
		if (++atTexture == textures.size())
			atTexture = 0;
	}
}
