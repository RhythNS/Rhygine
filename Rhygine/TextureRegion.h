#pragma once

class Texture;
class TextureRegion
{
public:
	float x, y;
	float width, height;
	Texture* texture;

	TextureRegion(float x, float y, float width, float height, Texture* texture) :
		x(x), y(y), width(width), height(height), texture(texture)
	{ }
private:
};
