#include "HighLoPch.h"
#include "FileMenu.h"

#ifdef HL_PLATFORM_WINDOWS
#include "Engine/Platform/Windows/WindowsFileMenu.h"
#endif  // HL_PLATFORM_WINDOWS

namespace highlo
{
	Ref<FileMenu> FileMenu::Create(const HLString &name)
	{
	#ifdef HL_PLATFORM_WINDOWS
		return MakeRef<WindowsFileMenu>(name);
	#else
		return nullptr;
	#endif
	}
}