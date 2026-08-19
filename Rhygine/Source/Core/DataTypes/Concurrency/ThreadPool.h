#pragma once

#include <memory>
#include <vector>
#include <deque>
#include <condition_variable>
#include <functional>
#include <future>
#include <type_traits>

#include "Thread.h"
#include "Mutex.h"

namespace Rhygine
{
	class ThreadPool
	{
	public:
		using Task = std::function<void()>;

		enum class Priority
		{
			Critical = 0,
			Normal = 1,
			Background = 2
		};

		ThreadPool(const ThreadPool&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;
		~ThreadPool() = default;

		static ThreadPool& GetInstance();
		static void Initialize(size_t t_threadCount = std::thread::hardware_concurrency());
		static void Shutdown();

		void Submit(Task t_task, Priority t_priority = Priority::Normal);

		template<typename F, typename... Args>
		auto Submit(F&& f, Args&&... args, Priority t_priority = Priority::Normal)
			-> std::future<std::invoke_result_t<F, Args...>>
		{
			using ReturnType = std::invoke_result_t<F, Args...>;
			auto task = std::make_shared<std::packaged_task<ReturnType()>>(
				std::bind(std::forward<F>(f), std::forward<Args>(args)...)
			);
			std::future<ReturnType> result = task->get_future();
			Submit([task]() { (*task)(); }, t_priority);
			return result;
		}

	private:
		static ThreadPool* s_instance;

		std::vector<std::unique_ptr<Thread>> m_threads;
		std::condition_variable_any m_condition;
		Mutex m_mutex;

		std::array<std::deque<Task>, 3> m_taskQueues;

		bool m_stop = false;

		ThreadPool(size_t t_threadCount);

		bool AllQueuesEmpty() const;
		void Work();
	};
}
