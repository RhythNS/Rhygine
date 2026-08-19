#pragma once

#include "WorldDescriptors.h"

namespace Rhygine
{
	class World;

	class Service
	{
		friend class World;

	public:
		virtual ~Service() = default;

		virtual void OnPreTick(World& t_world, DeltaTime t_dt) {}
		virtual void OnPostTick(World& t_world, DeltaTime t_dt) {}

		virtual void OnPostPhysicsTick(World& t_world, DeltaTime t_fixedDt) {}

		virtual bool IsRemovable() const { return false; }

		virtual void OnShutdown(World& t_world) {}

		TypeId GetTypeId() const { return m_typeId; }
	
	private:
		TypeId m_typeId = UINT32_MAX;
	};
}
