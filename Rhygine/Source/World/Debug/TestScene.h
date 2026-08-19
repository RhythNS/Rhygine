#pragma once

#include "Scene.h"

namespace Rhygine
{
	class World;

	class TestScene : public Scene
	{
	public:
		virtual void OnLoad(World& t_world) override;
	};
}
