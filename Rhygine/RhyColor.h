#pragma once

namespace RhyC
{
	typedef int color;

	constexpr color red =     0xFFFF0000;
	constexpr color green =   0xFF00FF00;
	constexpr color blue =    0xFF0000FF;
	constexpr color silver =  0xFFC0C0C0;
	constexpr color gray =    0xFF808080;
	constexpr color black =   0xFF000000;
	constexpr color white =   0xFFFFFFFF;
	constexpr color yellow =  0xFFFFFF00;
	constexpr color maroon =  0xFF800000;
	constexpr color orange =  0xFFFFA500;
	constexpr color olive =   0xFF808000;
	constexpr color aqua =    0xFF00FFFF;
	constexpr color teal =    0xFF008080;
	constexpr color navy =    0xFF000080;
	constexpr color fuchsia = 0xFFFF00FF;
	constexpr color purple =  0xFF800080;

	static color FromARGB(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
	{
		color col = b;
		col |= (g << 8);
		col |= (r << 16);
		col |= (a << 24);
		return col;
	}

	static void GetARGB(color& color, unsigned char& a, unsigned char& r, unsigned char& g, unsigned char& b)
	{
		a = (unsigned char)(color >> 24);
		r = (unsigned char)(color >> 16);
		g = (unsigned char)(color >> 8);
		b = (unsigned char)(color);
	}
}
