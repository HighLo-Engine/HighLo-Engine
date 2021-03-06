// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

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
		return Ref<WindowsMenuBar>::Create();
	#else
		return nullptr;
	#endif
	}
}

