#include "AABB.h"

bool Rhygine::AABB::Intersects(const AABB& other) const
{
	return (min.x <= other.max.x && max.x >= other.min.x) &&
		(min.y <= other.max.y && max.y >= other.min.y) &&
		(min.z <= other.max.z && max.z >= other.min.z);
}

bool Rhygine::AABB::Contains(const glm::vec3& point) const
{
	return (point.x >= min.x && point.x <= max.x) &&
		(point.y >= min.y && point.y <= max.y) &&
		(point.z >= min.z && point.z <= max.z);
}
