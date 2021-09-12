#pragma once
#include <RhyMath.h>

namespace RhyReact
{
    class ReactBodyBase;
    class ReactCollider;

	class RaycastInfo
	{
	public:
        RhyM::Vec3 worldPoint{ 0.0f,0.0f,0.0f };

        RhyM::Vec3 worldNormal{ 0.0f,0.0f,0.0f };

        float hitFraction = 1.0f;

        int meshSubpart = -1;

        int triangleIndex = -1;

        ReactBodyBase* body = nullptr;

        ReactCollider* collider = nullptr;
	};
}
