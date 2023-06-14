// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-10-21) initial release
//

#pragma once

#include "Engine/Threading/Thread.h"

// TEMP
// #define HL_PLATFORM_UNIX 1

#ifdef HL_PLATFORM_UNIX

#include <pthread.h>

namespace highlo
{
	class UnixThread : public Thread
	{
	public:

		HLAPI UnixThread(Runnable *runnable, const HLString &name, ThreadPriority priority);
		HLAPI ~UnixThread();

		HLAPI virtual int32 Run() override;
		HLAPI virtual bool Start(uint32 stackSize) override;
		HLAPI virtual void Kill(bool waitForJoin = false) override;
		HLAPI virtual void Join() override;
		HLAPI virtual void SetPriority(ThreadPriority priority) override;

		HLAPI virtual bool IsRunning() const override { return m_Running; }
		HLAPI virtual ThreadPriority GetPriority() override { return m_Priority; }
		HLAPI virtual UUID GetID() const override { return m_ID; }
		HLAPI virtual const HLString &GetName() const override { return m_Name; }

		HLAPI virtual uint32 GetStackSize(uint32 stackSize)
		{
			return stackSize;
		}

		HLAPI virtual int32 GetThreadPriority(ThreadPriority priority)
		{
			switch (priority)
			{
				case ThreadPriority::Highest:
					return 30;

				case ThreadPriority::AboveNormal:
					return 25;

				case ThreadPriority::Normal:
					return 15;

				case ThreadPriority::BelowNormal:
					return 5;

				case ThreadPriority::Lowest:
					return 1;
			}

			return 0;
		}

	private:

		static void *ThreadProc(void *pThis);
		static UnixThread *Setup(UnixThread *thread, uint32 stackSize = 0);

		void SetPriorityInternal(ThreadPriority priority);
		void ClearHandleInternal();
		void KillInternal(bool waitForJoin);

		HLString m_Name;
		UUID m_ID;
		ThreadPriority m_Priority = ThreadPriority::Normal;
		bool m_Running = false;
		bool m_CallAfterExit = true;
		Runnable *m_Runnable;

		pthread_t m_Handle;
		pthread_attr_t m_Attributes;
	};
}

#endif  // HL_PLATFORM_UNIX

