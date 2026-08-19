#pragma once

#include "World.h"

namespace Rhygine
{
	class Service
	{
	public:
		virtual ~Service() = default;

        virtual void OnPreTick(World& t_world, float t_dt) {}
        virtual void OnPostTick(World& t_world, float t_dt) {}

		virtual void OnPrePhysicsTick(World& t_world, float t_fixedDt) {}
		virtual void OnPostPhysicsTick(World& t_world, float t_fixedDt) {}

        virtual void OnShutdown(World& t_world) {}
	};
}
