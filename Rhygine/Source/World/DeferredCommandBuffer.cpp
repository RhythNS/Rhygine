#include "DeferredCommandBuffer.h"

#include "World.h"
#include "Debug/Error.h"

namespace
{
	template <typename... Ts>
	struct Overload : Ts...
	{
		using Ts::operator()...;
	};
}

namespace Rhygine
{
	void DeferredCommandBuffer::QueueRemoveComponent(EntityId t_id, TypeId t_type)
	{
		m_commands.push_back(Command
		{
			CommandType::Destroy,
			RemoveComponentCommand{ t_id, t_type }
		});
	}

	void DeferredCommandBuffer::QueueDestroyEntity(EntityId t_id)
	{
		m_commands.push_back(Command
		{
			CommandType::Destroy,
			DestroyEntityCommand{ t_id }
		});
	}

	void DeferredCommandBuffer::QueueCreateEntity(EntityId t_entityId)
	{
		m_commands.push_back(Command
		{
			CommandType::Create,
			CreateEntityCommand{ t_entityId }
		});
	}

	void DeferredCommandBuffer::QueueAddComponent(EntityId t_id, TypeId t_type, std::unique_ptr<std::byte[]> t_data)
	{
		m_commands.push_back(Command
		{
			CommandType::Create,
			AddComponentCommand{ t_id, t_type, std::move(t_data) }
		});
	}

	void DeferredCommandBuffer::QueueAddBehaviour(EntityId t_id, TypeId t_type, void* t_args)
	{
		m_commands.push_back(Command
		{
			CommandType::Create,
			AddBehaviourCommand{ t_id, t_type, t_args }
		});
	}

	void DeferredCommandBuffer::QueueRemoveBehaviour(EntityId t_id, TypeId t_type)
	{
		m_commands.push_back(Command
		{
			CommandType::Destroy,
			RemoveBehaviourCommand{ t_id, t_type }
		});
	}

	void DeferredCommandBuffer::Flush(World& t_world, CommandType t_command)
	{
		std::vector<Command> remainingCommands;
		constexpr size_t kMaxIterations = 1000;
		size_t iteration = 0;

		while (m_commands.empty() == false)
		{
			if (iteration++ >= kMaxIterations)
			{
				STOP_EXECUTION_MESSAGE("Exceeded maximum iterations while flushing deferred commands. Possible circular dependency or unresolvable command.");
				break;
			}

			std::vector<Command> currentBatch = std::move(m_commands);
			m_commands.clear();

			bool anyExecuted = false;

			for (Command& cmd : currentBatch)
			{
				const bool shouldExecute = (static_cast<uint8_t>(cmd.type) & static_cast<uint8_t>(t_command)) != 0;
				if (shouldExecute == false)
				{
					remainingCommands.push_back(std::move(cmd));
					continue;
				}

				anyExecuted = true;

				std::visit(Overload{
					[&](RemoveComponentCommand& c)
					{
						t_world.RemoveComponentImmediate(c.entityId, c.componentType);
					},
					[&](RemoveBehaviourCommand& c)
					{
						t_world.RemoveBehaviourImmediate(c.entityId, c.behaviourType);
					},
					[&](DestroyEntityCommand& c)
					{
						t_world.DestroyEntityImmediate(c.entityId);
					},
					[&](CreateEntityCommand& c)
					{
						t_world.CreateEntityImmediate(c.entityId);
					},
					[&](AddComponentCommand& c)
					{
						t_world.AddComponentImmediate(c.entityId, c.componentType, c.data.get());
					},
					[&](AddBehaviourCommand& c)
					{
						t_world.AddBehaviourImmediate(c.entityId, c.behaviourType, c.args);
					}
				}, cmd.data);
			}

			if (anyExecuted == false)
			{
				break;
			}
		}

		if (remainingCommands.empty() == false)
		{
			m_commands.insert(
				m_commands.begin(),
				std::make_move_iterator(remainingCommands.begin()),
				std::make_move_iterator(remainingCommands.end())
			);
		}
	}

	void DeferredCommandBuffer::Clear()
	{
		m_commands.clear();
	}
}
