#pragma once

#include <vector>
#include <memory>
#include <variant>
#include <cstdint>
#include <cstddef>

#include "WorldDescriptors.h"

namespace Rhygine
{
	class World;

	class DeferredCommandBuffer
	{
	public:
		enum class CommandType : uint8_t
		{
			Create = 1 << 0,
			Destroy = 1 << 1,
			All = Create | Destroy
		};

		void QueueRemoveComponent(EntityId t_id, TypeId t_type);
		void QueueDestroyEntity(EntityId t_id);

		void QueueCreateEntity(EntityId t_entityId);
		void QueueAddComponent(EntityId t_id, TypeId t_type, std::unique_ptr<std::byte[]> t_data);
		
		void QueueAddBehaviour(EntityId t_id, TypeId t_type, void* t_args);
		void QueueRemoveBehaviour(EntityId t_id, TypeId t_type);

		void Flush(World& t_world, CommandType t_command = CommandType::All);
		void Clear();

	private:
		struct RemoveComponentCommand
		{
			EntityId entityId;
			TypeId componentType;
		};
		struct RemoveBehaviourCommand
		{
			EntityId entityId;
			TypeId behaviourType;
		};
		struct DestroyEntityCommand
		{
			EntityId entityId;
		};
		struct CreateEntityCommand
		{
			EntityId entityId;
		};
		struct AddComponentCommand
		{
			EntityId entityId;
			TypeId componentType;
			std::unique_ptr<std::byte[]> data;
		};
		struct AddBehaviourCommand
		{
			EntityId entityId;
			TypeId behaviourType;
			void* args;
		};
		struct Command
		{
			CommandType type;
			std::variant
			<
				RemoveComponentCommand,
				RemoveBehaviourCommand,
				DestroyEntityCommand,
				CreateEntityCommand,
				AddComponentCommand,
				AddBehaviourCommand
			> data;
		};

		std::vector<Command> m_commands;
	};
}
