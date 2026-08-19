#pragma once

#include "DataTypes/Math.h"

namespace Rhygine
{
	struct Transform
	{
		glm::vec3 position{ 0.0f, 0.0f, 0.0f };
		glm::quat rotation = glm::identity<glm::quat>();
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

		glm::mat4 worldMatrix = glm::mat4(1.0f);
		bool worldDirty = true;
	};
}
