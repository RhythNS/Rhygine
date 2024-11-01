#include "Locks.h"

Rhygine::Lock::Lock(ILockable& t_lockable) : m_lockable(t_lockable)
{
	m_lockable.Lock();
}

Rhygine::Lock::~Lock()
{
	m_lockable.Unlock();
}

Rhygine::SharedLock::SharedLock(SharedMutex& t_mutex) : m_mutex(t_mutex)
{
	m_mutex.Lock();
}

Rhygine::SharedLock::~SharedLock()
{
	m_mutex.Unlock();
}
