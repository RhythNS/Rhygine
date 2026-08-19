#pragma once

#include <cstdint>
#include <vector>

#include "DataTypes/Math.h"

namespace Rhygine
{
	class Buffer;
	class Pipeline;
	class ResourceSet;

	struct RenderableData
	{
		Buffer* vertexBuffer;
		Buffer* indexBuffer;
		uint32_t indexCount;
		Pipeline* pipeline;
		ResourceSet* materialSet;

		glm::mat4 worldTransform;

		AABB worldAABB;

		uint64_t sortKey;
	};

	struct LightData
	{
		enum class Type
		{
			Point,
			Directional,
			Spot
		};
		Type type;
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 color;
		float intensity;
		float radius;
	};

	struct RenderPacket
	{
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
		glm::vec3 cameraPosition;
		float nearPlane, farPlane;

		std::vector<RenderableData> renderables;
		std::vector<LightData>lights;

		uint32_t frameIndex;
	};
}
