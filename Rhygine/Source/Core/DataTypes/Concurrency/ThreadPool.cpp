#include "ThreadPool.h"

#include <tracy/Tracy.hpp>

#include "Debug/Error.h"

Rhygine::ThreadPool* Rhygine::ThreadPool::s_instance = nullptr;

Rhygine::ThreadPool& Rhygine::ThreadPool::GetInstance()
{
	return *s_instance;
}

void Rhygine::ThreadPool::Initialize(size_t t_threadCount)
{
	ZoneScoped;

	ASSERT_ERROR_MESSAGE(s_instance == nullptr, "ThreadPool is already initialized.");
	
	s_instance = new ThreadPool(t_threadCount);
}

Rhygine::ThreadPool::ThreadPool(size_t t_threadCount)
{
	ZoneScoped;

	m_threads.reserve(t_threadCount);
	for (size_t i = 0; i < t_threadCount; i++)
	{
		auto thread = std::make_unique<Thread>(&ThreadPool::Work, this, "Worker#" + std::to_string(i + 1));
		thread->Start();
		m_threads.push_back(std::move(thread));
	}
}

void Rhygine::ThreadPool::Shutdown()
{
	ZoneScoped;

	ASSERT_ERROR_MESSAGE(s_instance != nullptr, "ThreadPool is not initialized.");

	{
		std::unique_lock<Mutex> lock(s_instance->m_mutex);
		s_instance->m_stop = true;
		s_instance->m_condition.notify_all();
	}

	for (auto& thread : s_instance->m_threads)
	{
		thread->Join();
	}

	delete s_instance;
	s_instance = nullptr;
}

void Rhygine::ThreadPool::Submit(Task t_task, Priority t_priority)
{
	ZoneScoped;

	std::unique_lock<Mutex> lock(m_mutex);
	m_taskQueues[static_cast<size_t>(t_priority)].push_back(std::move(t_task));
	m_condition.notify_one();
}

bool Rhygine::ThreadPool::AllQueuesEmpty() const
{
	ZoneScoped;

	for (const auto& queue : m_taskQueues)
	{
		if (!queue.empty())
		{
			return false;
		}
	}
	return true;
}

void Rhygine::ThreadPool::Work()
{
	ZoneScoped;

	while (true)
	{
		ZoneScopedN("Rhygine::ThreadPool::Work::Loop");

		Task task;
		{
			std::unique_lock<Mutex> lock(m_mutex);
			m_condition.wait(lock, [this]() { return m_stop || !AllQueuesEmpty(); });
			if (m_stop && AllQueuesEmpty())
			{
				return;
			}
			for (size_t i = 0; i < m_taskQueues.size(); i++)
			{
				if (!m_taskQueues[i].empty())
				{
					task = std::move(m_taskQueues[i].front());
					m_taskQueues[i].pop_front();
					break;
				}
			}
		}
		if (task)
		{
			task();
		}
	}
}
