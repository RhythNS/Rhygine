#include "Mutex.h"

bool Rhygine::Mutex::TryLock()
{
    const auto acquired = m_mutex.try_lock();
#ifdef TRACY_NO_LOCKS
    return acquired;
#else
    m_ctx.AfterTryLock(acquired);
    return acquired;
#endif // TRACY_NO_LOCKS
    
}

void Rhygine::Mutex::Lock()
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

void Rhygine::Mutex::Unlock()
{
    m_mutex.unlock();
#ifndef TRACY_NO_LOCKS
    m_ctx.AfterUnlock();
#endif // TRACY_NO_LOCKS
}
