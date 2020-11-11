#pragma once

namespace RhyC
{
	typedef int color;

	constexpr color red =     0xFF0000FF;
	constexpr color green =   0x00FF00FF;
	constexpr color blue =    0x0000FFFF;
	constexpr color silver =  0xC0C0C0FF;
	constexpr color gray =    0x808080FF;
	constexpr color black =   0x000000FF;
	constexpr color white =   0xFFFFFFFF;
	constexpr color yellow =  0xFFFF00FF;
	constexpr color maroon =  0x800000FF;
	constexpr color olive =   0x808000FF;
	constexpr color aqua =    0x00FFFFFF;
	constexpr color teal =    0x008080FF;
	constexpr color navy =    0x000080FF;
	constexpr color fuchsia = 0xFF00FFFF;
	constexpr color purple =  0x800080FF;

	static color FromRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		return (r << 24) | (g << 16) | (b << 8) || a;
	}

	static void GetRGBA(color& color, unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a)
	{
		r = (unsigned char)(color >> 24);
		g = (unsigned char)(color >> 16);
		g = (unsigned char)(color >> 8);
		a = (unsigned char)(color);
	}
}
