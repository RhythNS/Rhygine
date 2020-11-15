#pragma once
#include "RhyMath.h"
#include "Texture.h"
#include "TextureRegion.h"

#include <memory>

class MonoFont
{
public:
	MonoFont(std::string imagePath, int charWidth, int charHeight);
	TextureRegion* GetRegion(char character);
	RhyM::Vec2 GetRegionSize();

private:
	inline void InitRegions(int charWidth, int charHeight, int xCount, int yCount);

	TextureRegion regions[95];
	std::unique_ptr<Texture> font;
};
