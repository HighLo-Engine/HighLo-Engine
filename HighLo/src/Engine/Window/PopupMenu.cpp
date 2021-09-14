// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "PopupMenu.h"

#ifdef HL_PLATFORM_WINDOWS
#include "Engine/Platform/Windows/WindowsPopupMenu.h"
#endif // HL_PLATFORM_WINDOWS

namespace highlo
{
	Ref<PopupMenu> PopupMenu::Create(const HLString &name)
	{
	#ifdef HL_PLATFORM_WINDOWS
		return Ref<WindowsPopupMenu>::Create(name);
	#else
		return nullptr;
	#endif
	}
}
