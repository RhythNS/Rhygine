#pragma once

#include "DataTypes/Math.h"
#include "Types.h"
#include "Transform.h"

namespace Rhygine
{
	struct Camera
	{
		float fovDegrees = 60.0f;
		float nearPlane = 0.1f;
		float farPlane = 1000.0f;
	};
	RHYGINE_COMPONENT_WITH_DEPENDENCIES(Camera, Transform);
}
