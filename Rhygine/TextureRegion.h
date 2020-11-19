#pragma once
#include "Texture.h"

/// <summary>
/// Helper class for getting a specific region out of a texture.
/// </summary>
class TextureRegion
{
public:
	float x, y;
	float width, height;
	Texture* texture;

	/// <summary>
	/// Null constructor. Sets all variables to 0 or nullptr.
	/// </summary>
	TextureRegion() : x(0.0f), y(0.0f), width(0.0f), height(0.0f), texture(nullptr)
	{ }

	/// <summary>
	/// Standard constructor.
	/// </summary>
	/// <param name="x">The x coord of the region.</param>
	/// <param name="y">The y coord of the region.</param>
	/// <param name="width">The width of the region.</param>
	/// <param name="height">The height of the region.</param>
	/// <param name="texture">A reference to the texture that the region is off.</param>
	TextureRegion(float x, float y, float width, float height, Texture* texture) :
		x(x), y(y), width(width), height(height), texture(texture)
	{ }

	/// <summary>
	/// Generates a texture region that fills a specified texture.
	/// </summary>
	/// <param name="texture">A reference to the texture that the region is off.</param>
	TextureRegion(Texture* texture) :
		x(0.0f), y(0.0f), width((float)texture->GetWidth()), height((float)texture->GetHeight()), texture(texture)
	{ }

	virtual ~TextureRegion() { }
};
