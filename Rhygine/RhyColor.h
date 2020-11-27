#pragma once

// Helper typedef to encapsulate a color.

namespace RhyC
{
	typedef int color;

	constexpr color red =     0xFF0000FF;
	constexpr color green =   0xFF00FF00;
	constexpr color blue =    0xFFFF0000;
	constexpr color silver =  0xFFC0C0C0;
	constexpr color gray =    0xFF808080;
	constexpr color black =   0xFF000000;
	constexpr color white =   0xFFFFFFFF;
	constexpr color yellow =  0xFF00FFFF;
	constexpr color maroon =  0xFF000080;
	constexpr color orange =  0xFF00A5FF;
	constexpr color olive =   0xFF008080;
	constexpr color aqua =    0xFFFFFF00;
	constexpr color teal =    0xFF808000;
	constexpr color navy =    0xFF800000;
	constexpr color fuchsia = 0xFFFF00FF;
	constexpr color purple =  0xFF800080;

	/// <summary>
	/// Creates a color from RGBA values.
	/// </summary>
	/// <param name="r">The red value.</param>
	/// <param name="g">The green value.</param>
	/// <param name="b">The blue value.</param>
	/// <param name="a">The alpha value.</param>
	/// <returns></returns>
	static color FromRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		color col = r;
		col |= (g << 8);
		col |= (b << 16);
		col |= (a << 24);
		return col;
	}

	/// <summary>
	/// Gets the RGBA values from a color.
	/// </summary>
	/// <param name="color">The color where the RGBA values need to be extracted from.</param>
	/// <param name="r">The red value.</param>
	/// <param name="g">The green value.</param>
	/// <param name="b">The blue value.</param>
	/// <param name="a">The alpha value.</param>
	static void GetRGBA(color& color, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a)
	{
		a = (unsigned char)(color >> 24);
		b = (unsigned char)(color >> 16);
		g = (unsigned char)(color >> 8);
		r = (unsigned char)(color);
	}
}
