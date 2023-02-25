// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "UnixThread.h"

#include "Engine/Threading/ThreadRegistry.h"

#ifdef HL_PLATFORM_UNIX

namespace highlo
{
	uint64 Thread::GetCurrentThreadID()
	{
		return static_cast<uint64>(pthread_self());
	}

	UnixThread::UnixThread(Runnable *runnable, const HLString &name, ThreadPriority priority)
	{
		m_Runnable = runnable;
		m_Name = name;
		m_Priority = priority;
		m_ID = UUID();
		m_Handle = 0;
	}
	
	UnixThread::~UnixThread()
	{
		HL_ASSERT(m_Handle == 0);
	}
	
	int32 UnixThread::Run()
	{
		HL_ASSERT(m_Runnable);
		int32 exitCode = 1;

		ThreadRegistry::Get()->Add(this);
		m_Runnable->OnThreadStart();
		m_Running = true;

		HL_CORE_INFO("Thread {0}, {1} started with priority {2}", m_Name, m_ID, m_Priority);

		if (m_Runnable->Init())
		{
			exitCode = m_Runnable->Run();
			if (m_CallAfterExit)
				m_Runnable->Exit();
		}

		// End
		if (m_CallAfterExit)
		{
			m_CallAfterExit = false;
			m_Runnable->AfterExit(false);
		}

		ClearHandleInternal();
		m_Running = false;
		ThreadRegistry::Get()->Remove(this);
		m_Runnable->OnThreadEnd(exitCode);

		return exitCode;
	}
	
	bool UnixThread::Start(uint32 stackSize)
	{
		return pthread_create(&m_Handle, m_Attributes, ThreadProc, this);
	}

	void UnixThread::Kill(bool waitForJoin)
	{
		if (!m_Running)
			return;

		HL_ASSERT(GetID());

		if (m_CallAfterExit && m_Runnable)
			m_Runnable->Stop();

		HL_CORE_INFO("Killing Thread {0} with ID {1}", *m_Name, m_ID);

		KillInternal(waitForJoin);

		if (m_CallAfterExit && m_Runnable)
			m_Runnable->AfterExit(true);

		m_Running = false;
		ThreadRegistry::Get()->Remove(this);
	}
	
	void UnixThread::Join()
	{
		pthread_join(m_Handle, nullptr);
	}

	void UnixThread::SetPriority(ThreadPriority priority)
	{
		if (m_Priority != priority)
		{
			m_Priority = priority;
			SetPriorityInternal(priority);
		}
	}

	void *UnixThread::ThreadProc(void *pThis)
	{
		UnixThread *thread = (UnixThread*)pThis;
		const int32 exitCode = thread->Run();
		return (void*)(uintptr)exitCode;
	}

	UnixThread *UnixThread::Setup(UnixThread *thread, uint32 stackSize)
	{
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		
		stackSize = thread->GetStackSize(stackSize);
		if (stackSize != 0)
			pthread_attr_setstacksize(&attr, stackSize);

		thread->m_Attributes = attr;
		const int32 result = thread->Start(attr);
		if (result != 0)
		{
			HL_CORE_ERROR("Failed to spawn a thread. Result code: {0}", result);
			delete thread;
			return nullptr;
		}

		thread->SetPriorityInternal(thread->GetPriority());
		return thread;
	}

	void UnixThread::SetPriorityInternal(ThreadPriority priority)
	{
		struct sched_param sched;
		memset(&sched, 0, sizeof(struct sched_param));
		int32 policy = SCHED_RR;
		pthread_getschedparam(_thread, &policy, &sched);
		sched.sched_priority = GetThreadPriority(priority);
		pthread_setschedparam(_thread, policy, &sched);
	}

	void UnixThread::ClearHandleInternal()
	{
		m_Handle = 0;
	}
	
	void UnixThread::KillInternal(bool waitForJoin)
	{
		if (waitForJoin)
			pthread_join(m_Handle, nullptr);

		pthread_kill(m_Handle, SIGKILL);
	}
}

#endif // HL_PLATFORM_UNIX

