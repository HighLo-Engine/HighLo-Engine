// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "WindowsThread.h"

#include "Engine/Threading/ThreadRegistry.h"

#ifdef HL_PLATFORM_WINDOWS

#include <Windows.h>

namespace highlo
{
#ifdef HIGHLO_ENABLE_THREAD_NAMING

#pragma pack(push, 8)

	typedef struct tagTHREADNAME_INFO
	{
		DWORD dwType;		// Must be 0x1000
		LPCSTR szName;		// Name of Thread
		DWORD dwThreadID;	// ThreadID
		DWORD dwFlags;		// Thread Flags
	} THREADNAME_INFO;

#pragma pack(pop)

#endif // HIGHLO_ENABLE_THREAD_NAMING

	uint64 Thread::GetCurrentThreadID()
	{
		return ::GetCurrentThreadId();
	}

	WindowsThread::WindowsThread(Runnable *runnable, const HLString &name, ThreadPriority priority)
	{
		HL_ASSERT(runnable);

		m_Runnable = runnable;
		m_Name = name;
		m_Priority = priority;
		m_Handle = nullptr;
		m_ID = UUID();
	}

	WindowsThread::~WindowsThread()
	{
		if (m_Handle)
		{
			delete m_Handle;
			m_Handle = nullptr;
		}
	}

	int32 WindowsThread::Run()
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

	bool WindowsThread::Start(uint32 stackSize)
	{
		DWORD id;
		m_Handle = (void*)CreateThread(nullptr, stackSize, (LPTHREAD_START_ROUTINE)ThreadProc, this, STACK_SIZE_PARAM_IS_A_RESERVATION, &id);
		if (!m_Handle)
			return true;

		return false;
	}

	void WindowsThread::Kill(bool waitForJoin)
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

	void WindowsThread::Join()
	{
		WaitForSingleObject((HANDLE)m_Handle, INFINITE);
	}

	void WindowsThread::SetPriority(ThreadPriority priority)
	{
		if (m_Priority != priority)
		{
			m_Priority = priority;
			SetPriorityInternal(priority);
		}
	}
	
	ULONG WindowsThread::ThreadProc(void *pThis)
	{
		WindowsThread *thread = (WindowsThread*)pThis;

		const int32 exitCode = thread->Run();
		return static_cast<ULONG>(exitCode);
	}
	
	void WindowsThread::SetPriorityInternal(ThreadPriority priority)
	{
		int32 windowsPrio;
		switch (priority)
		{
			case ThreadPriority::Lowest:
				windowsPrio = THREAD_PRIORITY_LOWEST;
				break;

			case ThreadPriority::BelowNormal:
				windowsPrio = THREAD_PRIORITY_BELOW_NORMAL;
				break;

			case ThreadPriority::AboveNormal:
				windowsPrio = THREAD_PRIORITY_ABOVE_NORMAL;
				break;

			case ThreadPriority::Highest:
				windowsPrio = THREAD_PRIORITY_HIGHEST;
				break;

			case ThreadPriority::Normal:
			default:
				windowsPrio = THREAD_PRIORITY_NORMAL;
				break;
		}

		::SetThreadPriority((HANDLE)m_Handle, windowsPrio);
	}
	
	void WindowsThread::KillInternal(bool waitForJoin)
	{
		if (waitForJoin)
			WaitForSingleObject((HANDLE)m_Handle, INFINITE);

		CloseHandle((HANDLE)m_Handle);
	}
	
	void WindowsThread::ClearHandleInternal()
	{
		m_Handle = nullptr;
	}
	
	void WindowsThread::SetThreadNameInternal(DWORD threadId, const char *threadName)
	{
	#ifdef HIGHLO_ENABLE_THREAD_NAMING

		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = threadName;
		info.dwThreadID = threadId;
		info.dwFlags = 0;

	#endif // HIGHLO_ENABLE_THREAD_NAMING
	}
}

#endif // HL_PLATFORM_WINDOWS

