#pragma once
#include "RhyMath.h"

namespace RhyM
{
	class Rect
	{
	public:
		float x, y, width, height;
		void Set(Rect& other);
		void Set(float x, float y, float width, float height);
		void SetPosition(RhyM::Vec2 pos);
		void SetSize(RhyM::Vec2 size);
		bool Overlaps(Rect& other);
		bool Overlaps(RhyM::Vec2 point);
	};
}

