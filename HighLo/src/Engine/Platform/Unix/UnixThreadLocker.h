#pragma once

#include "Engine/Threading/ThreadLocker.h"

#ifdef HL_PLATFORM_UNIX

#include <pthread.h>

namespace highlo
{
	class UnixThreadLocker : public ThreadLocker
	{
	public:

		HLAPI UnixThreadLocker();
		HLAPI ~UnixThreadLocker();

		HLAPI virtual void Lock() const override;
		HLAPI virtual bool TryLock() const override;
		HLAPI virtual void Unlock() const override;

	private:

		pthread_mutex_t m_Mutex;
		pthread_mutex_t *m_MutexPtr;
	#if HL_DEBUG
		pthread_t m_OwningThreadId;
	#endif
	};
}

#endif // HL_PLATFORM_UNIX

