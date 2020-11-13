#include "RhyMath.h"

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
