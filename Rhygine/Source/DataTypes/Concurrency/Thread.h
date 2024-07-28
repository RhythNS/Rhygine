#pragma once
#include <thread>
#include <string>
#include <functional>
#include <atomic>

namespace Rhygine
{
	class Thread
	{
		enum class Priority {
			High,
			Normal,
			Low
		};

	public:
		Thread(std::function<void()> t_function, std::string t_name = "", Priority t_priority = Priority::Normal);
		Thread(const Thread&) = delete;
		Thread& operator=(const Thread&) = delete;

		void Start();
		bool IsStarted() const;
		void Join();
		bool IsFinished() const;
		Priority GetPriority() const;
		const std::string& GetName() const;

	private:
		std::function<void()> m_function;
		std::string m_name;
		Priority m_priority;

		std::thread m_thread;
		std::atomic<bool> m_started = false;
		std::atomic<bool> m_finished = false;
	};
}
