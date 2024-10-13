#pragma once

namespace Rhygine
{
	class ILockable
	{
	public:
		virtual bool TryLock() = 0;
		virtual void Lock() = 0;
		virtual void Unlock() = 0;
	};
}
