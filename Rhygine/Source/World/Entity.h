#pragma once
#include "WorldDescriptors.h"

namespace Rhygine
{
	class World;

	class Entity
	{
	public:
		Entity(EntityId t_id, World& t_world) : m_id(t_id), m_world(t_world) {}
		
		template <typename T>
		T* AddComponent(T&& t_data);
		void* AddComponent(TypeId t_type, const void* t_data);

		template <typename T>
		T* GetComponent() const;
		void* GetComponent(TypeId t_type) const;

		template <typename T>
		bool HasComponent() const;
		bool HasComponent(TypeId t_type) const;

		template <typename T>
		bool RemoveComponent();
		bool RemoveComponent(TypeId t_type);

		template <typename T, typename... Args>
		void AddBehaviour(Args&&... t_args);
		void AddBehaviour(TypeId t_type, void* t_args);

		template <typename T>
		T* GetBehaviour() const;
		void* GetBehaviour(TypeId t_type) const;

		template <typename T>
		bool HasBehaviour() const;
		bool HasBehaviour(TypeId t_type) const;

		template <typename T>
		bool RemoveBehaviour();
		bool RemoveBehaviour(TypeId t_type);

		[[nodiscard]] EntityId GetId() const { return m_id; }
		[[nodiscard]] World& GetWorld() const { return m_world; }

		void Destroy();
		[[nodiscard]] bool IsAlive() const;

	private:
		EntityId m_id;
		World& m_world;
	};
}
