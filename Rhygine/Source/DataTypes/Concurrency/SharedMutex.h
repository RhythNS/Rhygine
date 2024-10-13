#pragma once
#include <shared_mutex>
#ifndef TRACY_NO_LOCKS
#include <tracy/Tracy.hpp>
#include <client/TracyLock.hpp>
#endif // !TRACY_NO_LOCKS

#include "ILockable.h"

namespace Rhygine
{
	class SharedMutex : public ILockable
	{
	public:
		SharedMutex() = default;
		SharedMutex(const SharedMutex&) = delete;
		SharedMutex& operator=(const SharedMutex&) = delete;

		bool TryLock();
		void Lock();
		void Unlock();

		bool TryLockShared();
		void LockShared();
		void UnlockShared();

	private:
		std::shared_mutex m_mutex;
#ifndef TRACY_NO_LOCKS
		tracy::SharedLockableCtx m_ctx{ []() -> const tracy::SourceLocationData* { static constexpr tracy::SourceLocationData srcloc { nullptr, "SharedMutex", TracyFile, TracyLine, 0 }; return &srcloc; }() };
#endif // !TRACY_NO_LOCKS
	};
}
