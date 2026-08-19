#pragma once

#include "Entity.h"

namespace Rhygine
{
	class Behaviour
	{
		friend class World;

	public:
		virtual ~Behaviour() = default;

		virtual void OnActivate(Entity t_entity) {}

		virtual void OnPreTick(Entity t_entity, float t_dt) {}
		virtual void OnPostTick(Entity t_entity, float t_dt) {}

		virtual void OnPrePhysicsTick(Entity t_entity, float t_fixedDt) {}
		virtual void OnPostPhysicsTick(Entity t_entity, float t_fixedDt) {}

		virtual void OnDestroy(Entity t_entity) {}

		// virtual void OnMessage(Entity t_entity, World& t_world, const MessageEnvelope& t_message) {}

		bool IsMarkedForRemoval() const { return m_markedForRemoval; }
		void MarkForRemoval() { m_markedForRemoval = true; }

		virtual void OnPause(Entity t_entity) {}
		virtual void OnResume(Entity t_entity) {}

		bool IsActivated() const { return m_activated; }
		bool IsPaused() const { return m_paused; }
		TypeId GetTypeId() const { return m_typeId; }

	private:
		bool m_markedForRemoval = false;
		bool m_activated = false;
		bool m_paused = false;
		TypeId m_typeId = UINT32_MAX;
	};
}
