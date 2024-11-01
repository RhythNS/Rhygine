#include "SharedMutex.h"

bool Rhygine::SharedMutex::TryLock()
{
    const bool acquired = m_mutex.try_lock();
#ifdef TRACY_NO_LOCKS
    return acquired;
#else
    m_ctx.AfterTryLock(acquired);
    return acquired;
#endif // TRACY_NO_LOCKS
}

void Rhygine::SharedMutex::Lock()
{
#ifdef TRACY_NO_LOCKS
    m_mutex.lock();
#else
    const auto runAfter = m_ctx.BeforeLock();
    m_mutex.lock();
    if (runAfter)
    {
        m_ctx.AfterLock();
    }
#endif // TRACY_NO_LOCKS
}

void Rhygine::SharedMutex::Unlock()
{
    m_mutex.unlock();
#ifndef TRACY_NO_LOCKS
    m_ctx.AfterUnlock();
#endif // TRACY_NO_LOCKS
}

bool Rhygine::SharedMutex::TryLockShared()
{
    const bool acquired = m_mutex.try_lock_shared();
#ifdef TRACY_NO_LOCKS
    return acquired;
#else
    m_ctx.AfterTryLockShared(acquired);
    return acquired;
#endif // TRACY_NO_LOCKS
}

void Rhygine::SharedMutex::LockShared()
{
#ifdef TRACY_NO_LOCKS
    m_mutex.lock_shared();
#else
    const auto runAfter = m_ctx.BeforeLockShared();
    m_mutex.lock_shared();
    if (runAfter)
    {
        m_ctx.AfterLockShared();
    }
#endif // TRACY_NO_LOCKS
}

void Rhygine::SharedMutex::UnlockShared()
{
    m_mutex.unlock_shared();
#ifndef TRACY_NO_LOCKS
    m_ctx.AfterUnlockShared();
#endif // TRACY_NO_LOCKS
}
