#pragma once
#include <random>
#include <cassert>
#include <ctime>

namespace RhyM
{
	static std::mt19937 rng(static_cast<unsigned>(time(0)));

	/// <summary>
	/// Returns a float from 0.0 to 1.0
	/// </summary>
	static float GetRandomFloat()
	{
		return  static_cast<float>(rand()) / static_cast<float>(RAND_MAX); ;
	}

	/// <summary>
	/// Returns a float from specified min to specified max.
	/// </summary>
	static float GetRandomFloat(float min, float max)
	{
		assert(min <= max);
		return min + (static_cast<float>(rng() / (static_cast<float>(RAND_MAX) / (max - min))));
	}

}
