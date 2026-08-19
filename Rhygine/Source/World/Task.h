#pragma once

#include <string>
#include <vector>

#include  "WorldDescriptors.h"

namespace Rhygine
{
	class World;

	enum class TaskPhase
	{
		Physics,
		Normal
	};


	struct TaskAccessDescriptor
	{
		std::vector<TypeId> reads;
		std::vector<TypeId> writes;
	};

	class Task
	{
	public:
		virtual ~Task() = default;

		virtual std::string GetName() const = 0;

		virtual TaskPhase GetPhase() const { return TaskPhase::Normal; }

		const TaskAccessDescriptor& GetAccessDescriptor() const { return m_accessDescriptor; }

		virtual std::vector<TypeId> RunAfter() const { return {}; }
		virtual std::vector<TypeId> RunBefore() const { return {}; }

		virtual void Execute(World& t_world, float t_dt) = 0;

	protected:
		TaskAccessDescriptor m_accessDescriptor;
	};
}
