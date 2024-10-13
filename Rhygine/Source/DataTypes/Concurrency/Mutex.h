#pragma once
#include <mutex>
#ifndef TRACY_NO_LOCKS
#include <tracy/Tracy.hpp>
#include <client/TracyLock.hpp>
#endif // !TRACY_NO_LOCKS


#include "ILockable.h"

namespace Rhygine
{
	class Mutex : public ILockable
	{
	public:
		Mutex() = default;
		Mutex(const Mutex&) = delete;
		Mutex& operator=(const Mutex&) = delete;

		bool TryLock();
		void Lock();
		void Unlock();

	private:
		std::mutex m_mutex;
#ifndef TRACY_NO_LOCKS
		tracy::LockableCtx m_ctx{ []() -> const tracy::SourceLocationData* { static constexpr tracy::SourceLocationData srcloc { nullptr, "Mutex", TracyFile, TracyLine, 0 }; return &srcloc; }() };
#endif // !TRACY_NO_LOCKS
	};
}
