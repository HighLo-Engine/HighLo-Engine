// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-10-21) initial release
//

#pragma once

#include "Engine/Threading/ThreadLocker.h"

#ifdef HL_PLATFORM_WINDOWS

namespace highlo
{
	class WindowsThreadLocker : public ThreadLocker
	{
	public:

		WindowsThreadLocker();
		~WindowsThreadLocker();

		virtual void Lock() const override;
		virtual bool TryLock() const override;
		virtual void Unlock() const override;

	private:

		mutable CRITICAL_SECTION m_Lock;
	};
}

#endif // HL_PLATFORM_WINDOWS

