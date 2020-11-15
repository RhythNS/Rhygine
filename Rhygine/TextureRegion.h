#pragma once
#include "Texture.h"

class TextureRegion
{
public:
	float x, y;
	float width, height;
	Texture* texture;

	TextureRegion() : x(0.0f), y(0.0f), width(0.0f), height(0.0f), texture(nullptr)
	{ }

	TextureRegion(float x, float y, float width, float height, Texture* texture) :
		x(x), y(y), width(width), height(height), texture(texture)
	{ }

	TextureRegion(Texture* texture) :
		x(0.0f), y(0.0f), width(texture->GetWidth()), height(texture->GetHeight()), texture(texture)
	{ }
};
