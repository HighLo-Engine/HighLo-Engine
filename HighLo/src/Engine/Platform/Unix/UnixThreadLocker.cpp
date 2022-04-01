#include "HighLoPch.h"
#include "UnixThreadLocker.h"

#ifdef HL_PLATFORM_UNIX

namespace highlo
{
	UnixThreadLocker::UnixThreadLocker()
	{
		pthread_mutexattr_t attributes;
		pthread_mutexattr_init(&attributes);
		pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_RECURSIVE);

		pthread_mutex_init(&m_Mutex, &attributes);
		pthread_mutexattr_destroy(&attributes);
		m_MutexPtr = &m_Mutex;

	#if HL_DEBUG
		m_OwningThreadId = 0;
	#endif
	}
	
	UnixThreadLocker::~UnixThreadLocker()
	{
		pthread_mutex_destroy(&m_Mutex);
	}
	
	void UnixThreadLocker::Lock() const
	{
		pthread_mutex_lock(m_MutexPtr);
	#if HL_DEBUG
		((UnixThreadLocker*)this)->m_OwningThreadId = pthread_self();
	#endif
	}
	
	bool UnixThreadLocker::TryLock() const
	{
		return pthread_mutex_trylock(m_MutexPtr) == 0;
	}
	
	void UnixThreadLocker::Unlock() const
	{
	#if HL_DEBUG
		((UnixThreadLocker*) this)->m_OwningThreadId = 0;
	#endif
		pthread_mutex_unlock(m_MutexPtr);
	}
}

#endif // HL_PLATFORM_UNIX

