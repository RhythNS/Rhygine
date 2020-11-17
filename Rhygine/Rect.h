#pragma once
#include "RhyMath.h"

namespace RhyM
{
	/// <summary>
	/// Represents a rectangle.
	/// </summary>
	class Rect
	{
	public:
		Rect(float x, float y, float width, float height) :
			x(x), y(y), width(width), height(height)
		{ }

		Rect() : x(0), y(0), width(0), height(0)
		{ }

		float x, y, width, height;
		/// <summary>
		/// Copies all values from a given rectangle.
		/// </summary>
		void Set(Rect& other);
		void Set(float x, float y, float width, float height);
		void SetPosition(RhyM::Vec2 pos);
		void SetSize(RhyM::Vec2 size);
		/// <summary>
		/// Checks if this and the given rectangle are overlapping.
		/// </summary>
		bool Overlaps(Rect& other);
		/// <summary>
		/// Checks if a point is inside this rectangle.
		/// </summary>
		bool Overlaps(RhyM::Vec2 point);
	};
}

