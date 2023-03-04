// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-10-21) initial release
//

#pragma once

#include "Engine/Threading/Thread.h"

#ifdef HL_PLATFORM_WINDOWS

namespace highlo
{
	class WindowsThread : public Thread
	{
	public:

		HLAPI WindowsThread(Runnable *runnable, const HLString &name, ThreadPriority priority);
		HLAPI ~WindowsThread();

		HLAPI virtual int32 Run() override;
		HLAPI virtual bool Start(uint32 stackSize) override;
		HLAPI virtual void Kill(bool waitForJoin = false) override;
		HLAPI virtual void Join() override;
		HLAPI virtual void SetPriority(ThreadPriority priority) override;

		HLAPI virtual bool IsRunning() const override { return m_Running; }
		HLAPI virtual ThreadPriority GetPriority() override { return m_Priority; }
		HLAPI virtual UUID GetID() const override { return m_ID; }
		HLAPI virtual const HLString &GetName() const override { return m_Name; }

	private:

		static ULONG ThreadProc(void *pThis);
		void SetPriorityInternal(ThreadPriority priority);
		void KillInternal(bool waitForJoin);
		void ClearHandleInternal();
		void SetThreadNameInternal(DWORD threadId, const char *threadName);

		HLString m_Name;
		UUID m_ID;
		ThreadPriority m_Priority = ThreadPriority::Normal;
		bool m_Running = false;
		bool m_CallAfterExit = true;
		Runnable *m_Runnable;
		void *m_Handle;
	};
}

#endif // HL_PLATFORM_WINDOWS

