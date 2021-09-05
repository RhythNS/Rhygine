#include <2D/TextureGenerator.h>
#include <RhyMath.h>

#include <cassert>
#include <vector>
#include <algorithm>

// Disable warning about arithmetic overflow and conversion loss of data
#pragma warning(push)
#pragma warning(disable : 26451 4244)

TextureGenerator::TextureGenerator(int width, int height) : width(width), height(height)
{
	tex.resize(static_cast<size_t>(width) * static_cast<size_t>(height), brush);
}

TextureGenerator::TextureGenerator(int width, int height, RhyC::color initialColor) : width(width), height(height)
{
	tex.resize(static_cast<size_t>(width) * static_cast<size_t>(height), initialColor);
}

void TextureGenerator::Fill(int x, int y)
{
	assert(x >= 0 && x < width && y >= 0 && y < height);

	RhyC::color toReplace;
	// if the replace color is the same as the brush, simply return
	if ((toReplace = tex[x * height + y]) == brush)
		return;

	std::vector<RhyM::Vec2I> openSet, closedSet;

	// put the first pixel to the openset
	openSet.push_back(RhyM::Vec2I(x,y));

	// while the openset is not empty
	while (openSet.size() != 0)
	{
		// take a pixel out of the openset and add it to the closedset.
		auto currentIt = openSet.begin();
		RhyM::Vec2I current = *currentIt;
		closedSet.push_back(current);
		openSet.erase(currentIt);

		// Check out neighbour pixels
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

			// Pixel should be replaced, so add it to the openset.
			openSet.push_back(toExamine[i]);
		}
	}

	// Replace every pixel in the closed set with the brush pixel.
	for (auto point : closedSet)
	{
		tex[point.x * height + point.y] = brush;
	}

}

void TextureGenerator::DrawPixel(int x, int y)
{
	assert(x >= 0 && x < width&& y >= 0 && y < height);
	tex[x * height + y] = brush;
}

int TextureGenerator::GetWidth()
{
	return width;
}

int TextureGenerator::GetHeight()
{
	return height;
}

std::vector<RhyC::color>* TextureGenerator::GetTexture()
{
	return &tex;
}

#pragma warning(pop)
