#include <Bindables/TextureChanger.h>
#include <ECS/Bindable/Texture.h>
#include <Core/Window.h>

#include <string>

TextureChanger::TextureChanger()
{
	for (int i = 1; i < 21; i++)
	{
		textures.push_back(std::make_unique<Texture>(("Assets\\TestModels\\Sprite\\Walk (" + std::to_string(i) + ").png").c_str(), 0));
	}
}

void TextureChanger::Bind()
{
	textures[atTexture]->Bind();
}

void TextureChanger::Update()
{
	// Subtract the timer with the delta time.
	timer -= Window::GetInstance()->time.GetDelta();
	// If the timer is under 0, increment the atTexture. 
	if (timer <= 0)
	{
		timer = timeToChange;
		// If atTexture is outside the bounds of the array, make it wrap around back to 0.
		if (++atTexture == textures.size())
			atTexture = 0;
	}
}
