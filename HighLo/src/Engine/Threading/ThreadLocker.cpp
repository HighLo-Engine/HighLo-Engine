#include "HighLoPch.h"
#include "ThreadLocker.h"

#ifdef HL_PLATFORM_WINDOWS
#include "Engine/Platform/Windows/WindowsThreadLocker.h"
#elif HL_PLATFORM_UNIX
#include "Engine/Platform/Unix/UnixThreadLocker.h"
#endif // HL_PLATFORM_WINDOWS

namespace highlo
{
	ThreadLocker *ThreadLocker::Create()
	{
	#ifdef HL_PLATFORM_WINDOWS
		return new WindowsThreadLocker();
	#elif HL_PLATFORM_UNIX
		return new UnixThreadLocker();
	#else
		return nullptr;
	#endif // HL_PLATFORM_WINDOWS
	}
}

