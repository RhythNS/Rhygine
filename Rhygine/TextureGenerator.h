#pragma once
#include "RhyColor.h"

#include <vector>

class TextureGenerator
{
public:
	TextureGenerator(size_t width, size_t height);
	TextureGenerator(size_t width, size_t height, RhyC::color initialColor);
	void Fill(size_t x, size_t y);
	//void DrawLine(int xFrom, int yFrom, int xTo, int yTo);
	void DrawPixel(size_t x, size_t y);

	size_t GetWidth();
	size_t GetHeight();

	std::vector<RhyC::color>* GetTexture();

	RhyC::color brush = RhyC::white;
private:
	std::vector<RhyC::color> tex;
	size_t width, height;
};
