#include "MonoFont.h"

MonoFont::MonoFont(std::string imagePath, int charWidth, int charHeight)
{
	font = std::make_unique<Texture>(imagePath.c_str(), 0);

	int xCount = font->GetWidth() / charWidth;
	int yCount = font->GetHeight() / charHeight;

	InitRegions(charWidth, charHeight, xCount, yCount);

	regions[0] = TextureRegion((xCount - 1) * charWidth, (yCount - 1) * charHeight, charWidth, charHeight, font.get());
}

TextureRegion* MonoFont::GetRegion(char character)
{
	assert(character > 31 && character < 127);
	return &regions[character - 32];
}

RhyM::Vec2 MonoFont::GetRegionSize()
{
	return RhyM::Vec2(regions[0].width, regions[0].height);
}

inline void MonoFont::InitRegions(int charWidth, int charHeight, int xCount, int yCount)
{
	int atCount = 0;
	for (int y = 0; y < yCount; y++)
	{
		for (int x = 0; x < xCount; x++)
		{
			regions[++atCount] = TextureRegion(x * charWidth, y * charHeight, charWidth, charHeight, font.get());

			if (atCount == 94)
				return;
		}
	}
}
