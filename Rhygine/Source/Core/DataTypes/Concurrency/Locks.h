#pragma once

#include "ILockable.h"
#include "SharedMutex.h"

namespace Rhygine
{
	class Lock
	{
	public:
		Lock(ILockable& t_lockable);
		~Lock();

	private:
		ILockable& m_lockable;
	};

	class SharedLock
	{
	public:
		SharedLock(SharedMutex& t_mutex);
		~SharedLock();

	private:
		SharedMutex& m_mutex;
	};
}
