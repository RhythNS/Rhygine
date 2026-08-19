#pragma once
#include <thread>
#include <string>
#include <functional>
#include <atomic>
#include <utility>

namespace Rhygine
{
	class Thread
	{
	public:
		enum class Priority {
			High,
			Normal,
			Low
		};

		Thread(std::function<void()> t_function, std::string t_name = "", Priority t_priority = Priority::Normal);

		template<typename Class>
		Thread(void (Class::*t_method)(), Class* t_instance, std::string t_name = "", Priority t_priority = Priority::Normal)
			: Thread([t_instance, t_method]() { (t_instance->*t_method)(); }, std::move(t_name), t_priority)
		{
		}

		template<typename Class>
		Thread(void (Class::*t_method)(), Class& t_instance, std::string t_name = "", Priority t_priority = Priority::Normal)
			: Thread(t_method, &t_instance, std::move(t_name), t_priority)
		{
		}

		template<typename Class>
		Thread(void (Class::*t_method)() const, const Class* t_instance, std::string t_name = "", Priority t_priority = Priority::Normal)
			: Thread([t_instance, t_method]() { (t_instance->*t_method)(); }, std::move(t_name), t_priority)
		{
		}

		template<typename Class>
		Thread(void (Class::*t_method)() const, const Class& t_instance, std::string t_name = "", Priority t_priority = Priority::Normal)
			: Thread(t_method, &t_instance, std::move(t_name), t_priority)
		{
		}

		template<typename Class>
		Thread(Class* t_instance, void (Class::*t_method)(), std::string t_name = "", Priority t_priority = Priority::Normal)
			: Thread(t_method, t_instance, std::move(t_name), t_priority)
		{
		}

		template<typename Class>
		Thread(Class& t_instance, void (Class::*t_method)(), std::string t_name = "", Priority t_priority = Priority::Normal)
			: Thread(t_method, &t_instance, std::move(t_name), t_priority)
		{
		}

		template<typename Class>
		Thread(const Class* t_instance, void (Class::*t_method)() const, std::string t_name = "", Priority t_priority = Priority::Normal)
			: Thread(t_method, t_instance, std::move(t_name), t_priority)
		{
		}

		template<typename Class>
		Thread(const Class& t_instance, void (Class::*t_method)() const, std::string t_name = "", Priority t_priority = Priority::Normal)
			: Thread(t_method, &t_instance, std::move(t_name), t_priority)
		{
		}

		Thread(const Thread&) = delete;
		Thread& operator=(const Thread&) = delete;

		void Start();
		[[nodiscard]] bool IsStarted() const;
		void Join();
		[[nodiscard]] bool IsFinished() const;
		[[nodiscard]] Priority GetPriority() const;
		[[nodiscard]] const std::string& GetName() const;

	private:
		std::function<void()> m_function;
		std::string m_name;
		Priority m_priority;

		std::thread m_thread;
		std::atomic<bool> m_started = false;
		std::atomic<bool> m_finished = false;
	};
}
