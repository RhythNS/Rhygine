#pragma once

#include "DataTypes/Math.h"

namespace Rhygine
{
	struct AABB
	{
		glm::vec3 min;
		glm::vec3 max;

		AABB() : min(glm::vec3(0.0f)), max(glm::vec3(0.0f)) {}
		AABB(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}

		bool Intersects(const AABB& other) const;
		bool Contains(const glm::vec3& point) const;
	};
}
