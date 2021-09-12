#pragma once
#include <RhyMath.h>

namespace RhyReact
{
	struct Ray
	{
	public:
		RhyM::Vec3 origin;

		RhyM::Vec3 endPoint;

		float maxFraction;

		Ray(RhyM::Vec3  origin, RhyM::Vec3 endPoint, float maxFraction = 1.0f)
			: origin(origin), endPoint(endPoint), maxFraction(maxFraction)
		{

		}
	};
}
