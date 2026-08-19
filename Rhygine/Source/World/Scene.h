#pragma once
#include <cstdint>

namespace Rhygine
{
	class World;

	enum class SceneState : uint8_t
	{
		Unloaded,
		Loading,
		Loaded,
		Active,
		Unloading
	};

	class Scene
	{
	public:
		virtual ~Scene() = default;

		// TODO: Delete once Scene can be loaded from a file
		virtual void OnLoad(World& t_world) = 0;
	};
}
