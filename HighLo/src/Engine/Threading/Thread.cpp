// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Thread.h"

#include "Engine/Application/Application.h"

#ifdef HL_PLATFORM_WINDOWS
#include "Engine/Platform/Windows/WindowsThread.h"
#endif // HL_PLATFORM_WINDOWS

namespace highlo
{
	bool Thread::IsInMainThread()
	{
		return HLApplication::Get().GetApplicationSettings().MainThreadID == GetCurrentThreadID();
	}
	
	Ref<Thread> Thread::Create(Runnable *runnable, const HLString &name, ThreadPriority priority, uint32 stackSize)
	{
		Ref<Thread> result = nullptr;

	#ifdef HL_PLATFORM_WINDOWS
		result = Ref<WindowsThread>::Create(runnable, name, priority);
	#endif // HL_PLATFORM_WINDOWS

		if (result->Start(stackSize))
			result = nullptr;

		return result;
	}
}

