#pragma once
#include <RhyMath.h>
#include <ECS/Bindable/Texture.h>
#include <2D/TextureRegion.h>

#include <memory>

/// <summary>
/// A mono-spaced font container which can be accessed to get single characters. Only these
/// characters are supported:
///  !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
/// </summary>
class MonoFont
{
public:
	/// <summary>
	/// Standard constructor.
	/// </summary>
	/// <param name="imagePath">The path to the font image.</param>
	/// <param name="charWidth">The width of a single character.</param>
	/// <param name="charHeight">The height of a single character.</param>
	MonoFont(std::string imagePath, int charWidth, int charHeight);
	virtual ~MonoFont() { }
	/// <summary>
	/// Gets a specified character.
	/// </summary>
	/// <param name="character">The letter/ character to be gotten.</param>
	/// <returns>The letter/ character as a TextureRegion.</returns>
	TextureRegion* GetRegion(char character);
	/// <summary>
	/// Gets the size of a single character.
	/// </summary>
	RhyM::Vec2 GetRegionSize();

private:
	/// <summary>
	/// Helper method for initialization.
	/// </summary>
	inline void InitRegions(int charWidth, int charHeight, int xCount, int yCount);

	/// <summary>
	/// ASCII code 32-126
	/// </summary>
	TextureRegion regions[95];
	std::unique_ptr<Texture> font;
};
