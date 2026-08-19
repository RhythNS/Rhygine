#pragma once

#include <string>
#include <vector>

#include "DataTypes/Math.h"

namespace Rhygine
{
	struct MaterialParameter
	{
		enum class Type { Float, Vec2, Vec3, Vec4, Texture };
		std::string name;
		Type type;

		float floatValue = 0.0f;
		glm::vec2 vec2Value = {};
		glm::vec3 vec3Value = {};
		glm::vec4 vec4Value = {};
		std::string texturePath; // Resolved to a ResourceHandle<TextureResource> at load time
	};

	struct MaterialParameters
	{
		std::vector<MaterialParameter> parameters;
	};
}
