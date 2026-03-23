#pragma once
#include <shared_mutex>
#ifndef TRACY_NO_LOCKS
#include <tracy/Tracy.hpp>
#include <client/TracyLock.hpp>
#endif // !TRACY_NO_LOCKS

namespace Rhygine
{
	class SharedMutex
	{
	public:
		SharedMutex() = default;
		SharedMutex(const SharedMutex&) = delete;
		SharedMutex& operator=(const SharedMutex&) = delete;

		[[nodiscard]] bool TryLock();
		void Lock();
		void Unlock();

		[[nodiscard]] bool TryLockShared();
		void LockShared();
		void UnlockShared();

		[[nodiscard]] bool try_lock() { return TryLock(); }
		void lock() { Lock(); }
		void unlock() { Unlock(); }

		[[nodiscard]] bool try_lock_shared() { return TryLockShared(); }
		void lock_shared() { LockShared(); }
		void unlock_shared() { UnlockShared(); }

	private:
		std::shared_mutex m_mutex;
#ifndef TRACY_NO_LOCKS
		tracy::SharedLockableCtx m_ctx{ []() -> const tracy::SourceLocationData* { static constexpr tracy::SourceLocationData srcloc { nullptr, "SharedMutex", TracyFile, TracyLine, 0 }; return &srcloc; }() };
#endif // !TRACY_NO_LOCKS
	};
}
