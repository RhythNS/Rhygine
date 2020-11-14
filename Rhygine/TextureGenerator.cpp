#include "TextureGenerator.h"
#include "RhyMath.h"

#include <cassert>
#include <vector>
#include <algorithm>

TextureGenerator::TextureGenerator(size_t width, size_t height) : width(width), height(height)
{
	tex.resize(width * height, brush);
}

TextureGenerator::TextureGenerator(size_t width, size_t height, RhyC::color initialColor) : width(width), height(height)
{
	tex.resize(width * height, initialColor);
}

void TextureGenerator::Fill(size_t x, size_t y)
{
	assert(x >= 0 && x < width && y >= 0 && y < height);

	RhyC::color toReplace;
	if ((toReplace = tex[x * height + y]) == brush)
		return;

	std::vector<RhyM::Vec2I> openSet, closedSet;

	openSet.push_back(RhyM::Vec2I(x,y));
	while (openSet.size() != 0)
	{
		auto currentIt = openSet.begin();
		RhyM::Vec2 current = *currentIt;
		closedSet.push_back(current);
		openSet.erase(currentIt);

		RhyM::Vec2I toExamine[4];
		toExamine[0] = RhyM::Vec2I(current.x - 1, current.y);
		toExamine[1] = RhyM::Vec2I(current.x + 1, current.y);
		toExamine[2] = RhyM::Vec2I(current.x, current.y - 1);
		toExamine[3] = RhyM::Vec2I(current.x, current.y + 1);

		for (int i = 0; i < 4; i++)
		{
			// out of bounds?
			if (toExamine[i].x < 0 || toExamine[i].y < 0 || toExamine[i].x >= width || toExamine[i].y >= height)
				continue;

			// not the same color?
			if (tex[toExamine[i].x * height + toExamine[i].y] != toReplace)
				continue;

			// already in the closed or open list?
			if (std::find(closedSet.begin(), closedSet.end(), toExamine[i]) != closedSet.end() ||
				(openSet.size() != 0 && std::find(openSet.begin(), openSet.end(), toExamine[i]) != openSet.end()))
				continue;

			openSet.push_back(toExamine[i]);
		}
	}

	for (auto point : closedSet)
	{
		tex[point.x * height + point.y] = brush;
	}

}

void TextureGenerator::DrawPixel(size_t x, size_t y)
{
	assert(x >= 0 && x < width&& y >= 0 && y < height);
	tex[x * height + y] = brush;
}

size_t TextureGenerator::GetWidth()
{
	return width;
}

size_t TextureGenerator::GetHeight()
{
	return height;
}

std::vector<RhyC::color>* TextureGenerator::GetTexture()
{
	return &tex;
}
