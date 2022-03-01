// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "WindowsThreadLocker.h"

#ifdef HL_PLATFORM_WINDOWS

#include <Windows.h>

namespace highlo
{
	WindowsThreadLocker::WindowsThreadLocker()
	{
		InitializeCriticalSectionEx(&m_Lock, 100, 0x01000000);
	}

	WindowsThreadLocker::~WindowsThreadLocker()
	{
		DeleteCriticalSection(&m_Lock);
	}

	void WindowsThreadLocker::Lock() const
	{
		if (TryEnterCriticalSection(&m_Lock) == 0)
		{
			EnterCriticalSection(&m_Lock);
		}
	}

	bool WindowsThreadLocker::TryLock() const
	{
		return TryEnterCriticalSection(&m_Lock) != 0;
	}

	void WindowsThreadLocker::Unlock() const
	{
		LeaveCriticalSection(&m_Lock);
	}
}

#endif // HL_PLATFORM_WINDOWS

