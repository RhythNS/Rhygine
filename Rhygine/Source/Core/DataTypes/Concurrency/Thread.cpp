#include "Thread.h"

#include <tracy\Tracy.hpp>

Rhygine::Thread::Thread(std::function<void()> t_function, std::string t_name, Priority t_priority)
	: m_function(t_function), m_name(t_name), m_priority(t_priority)
{
}

void Rhygine::Thread::Start()
{
	// assert if already started?
	m_thread = std::thread([this] 
		{
#if TRACY_ENABLE
			static int counter = 0;
			std::string name = (m_name.size() > 0 ? m_name + " #" : "Unnamed #")
				+ std::to_string(std::this_thread::get_id()._Get_underlying_id());
			tracy::SetThreadName(name.c_str());
			ZoneScopedN("Thread");
#endif // TRACY_ENABLE
			m_started = true;
			m_function(); 
			m_finished = true;
		}
	);
}

bool Rhygine::Thread::IsStarted() const
{
	return m_started.load();
}

void Rhygine::Thread::Join()
{
	// assert if already started?if (m_thread.joinable()) {
	if (m_thread.joinable())
	{
		m_thread.join();
	}
}

bool Rhygine::Thread::IsFinished() const
{
	return m_finished.load();
}

Rhygine::Thread::Priority Rhygine::Thread::GetPriority() const
{
	return m_priority;
}

const std::string& Rhygine::Thread::GetName() const
{
	return m_name;
}
