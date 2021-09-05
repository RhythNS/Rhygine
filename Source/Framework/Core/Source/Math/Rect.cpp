#include <Math/Rect.h>

void RhyM::Rect::Set(Rect& other)
{
	x = other.x;
	y = other.y;
	width = other.width;
	height = other.height;
}

void RhyM::Rect::Set(float _x, float _y, float _width, float _height)
{
	x = _x;
	y = _y;
	width = _width;
	height = _height;
}

void RhyM::Rect::SetPosition(Vec2 pos)
{
	x = pos.x;
	y = pos.y;
}

void RhyM::Rect::SetSize(Vec2 size)
{
	width = size.x;
	height = size.y;
}

bool RhyM::Rect::Overlaps(Rect& other)
{
	return x < other.x + other.width && x + width > other.x &&
		y + height > other.y && y < other.y + height;
}

bool RhyM::Rect::Overlaps(Vec2 point)
{
	return point.x <= x + width && point.x >= x &&
		point.y <= y + height && point.y >= y;
}
