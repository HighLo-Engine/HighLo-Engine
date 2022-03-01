// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

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
		return Ref<WindowsFileMenu>::Create(name);
	#else
		return nullptr;
	#endif
	}
}
