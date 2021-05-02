#include "HighLoPch.h"
#include "MenuBar.h"

#ifdef HL_PLATFORM_WINDOWS
#include "Engine/Platform/Windows/WindowsMenuBar.h"
#endif // HL_PLATFORM_WINDOWS

namespace highlo
{
	Ref<MenuBar> MenuBar::Create()
	{
	#ifdef HL_PLATFORM_WINDOWS
		return MakeRef<WindowsMenuBar>();
	#else
		return nullptr;
	#endif
	}
}