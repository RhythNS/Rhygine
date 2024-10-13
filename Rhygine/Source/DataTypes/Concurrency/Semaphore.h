#pragma once
#include <semaphore>
#ifndef TRACY_NO_LOCKS
#include <tracy/Tracy.hpp>
#include <client/TracyLock.hpp>
#endif

#include "ILockable.h"

namespace Rhygine
{
	template<class T>
	class Semaphore : public ILockable
	{
	public:
		Semaphore() = delete;
		Semaphore(T t_initial_counter) : m_semaphore{ t_initial_counter } {}
		Semaphore(const Semaphore&) = delete;
		Semaphore& operator=(const Semaphore&) = delete;

		bool TryLock()
		{
			const auto acquired = m_semaphore.try_acquire();
#ifdef TRACY_NO_LOCKS
			return acquired;
#else
			m_ctx.AfterTryLock(acquired);
			return acquired;
#endif // TRACY_NO_LOCKS
		}

		void Lock()
		{
#ifdef TRACY_NO_LOCKS
			m_semaphore.acquire();
#else
			const auto runAfter = m_ctx.BeforeLock();
			m_semaphore.acquire();
			if (runAfter)
			{
				m_ctx.AfterLock();
			}
#endif // TRACY_NO_LOCKS
		}

		void Unlock()
		{
			m_semaphore.release();
#ifndef TRACY_NO_LOCKS
			m_ctx.AfterUnlock();
#endif // TRACY_NO_LOCKS
		}

	private:
		std::counting_semaphore<T> m_semaphore;
#ifndef TRACY_NO_LOCKS
		tracy::LockableCtx m_ctx{ []() -> const tracy::SourceLocationData* { static constexpr tracy::SourceLocationData srcloc { nullptr, "Semaphore", TracyFile, TracyLine, 0 }; return &srcloc; }() };
#endif // TRACY_NO_LOCKS
	};
}
