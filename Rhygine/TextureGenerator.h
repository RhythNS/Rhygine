#pragma once
#include "RhyColor.h"

#include <vector>

/// <summary>
/// Generates texture on runtime.
/// </summary>
class TextureGenerator
{
public:
	/// <summary>
	/// Standard constructor.
	/// </summary>
	/// <param name="width">The width of the texture.</param>
	/// <param name="height">The height of the texture.</param>
	TextureGenerator(size_t width, size_t height);
	/// <summary>
	/// Color constructor.
	/// </summary>
	/// <param name="width">The width of the texture.</param>
	/// <param name="height">The height of the texture.</param>
	/// <param name="initialColor">The initial color that each pixel of the texture has.</param>
	TextureGenerator(size_t width, size_t height, RhyC::color initialColor);
	/// <summary>
	/// Exchanges the color of all touching pixels that share the same color as the specified pixel with the
	/// brush color.
	/// </summary>
	/// <param name="x">The x coord of the pixel.</param>
	/// <param name="y">The y coord of the pixel.</param>
	void Fill(size_t x, size_t y);
	//void DrawLine(int xFrom, int yFrom, int xTo, int yTo);
	/// <summary>
	/// Replaces the color of a single pixel with the color of the brush.
	/// </summary>
	/// <param name="x">The x coord of the pixel.</param>
	/// <param name="y">The y coord of the pixel.</param>
	void DrawPixel(size_t x, size_t y);

	/// <summary>
	/// Gets the width.
	/// </summary>
	size_t GetWidth();
	/// <summary>
	/// Gets the height.
	/// </summary>
	size_t GetHeight();

	/// <summary>
	/// Gets the color array.
	/// </summary>
	/// <returns></returns>
	std::vector<RhyC::color>* GetTexture();

	/// <summary>
	/// The current brush color.
	/// </summary>
	RhyC::color brush = RhyC::white;
private:
	std::vector<RhyC::color> tex;
	size_t width, height;
};
