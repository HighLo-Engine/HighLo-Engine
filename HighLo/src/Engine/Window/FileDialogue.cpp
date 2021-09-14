// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "FileDialogue.h"

#ifdef HL_PLATFORM_WINDOWS
#include "Engine/Platform/Windows/WindowsFileDialogue.h"
#endif // HL_PLATFORM_WINDOWS

namespace highlo
{
	void FileDialogue::SetFilter(const FileDialogueFilter &filter)
	{
		m_Filter = filter;
	}

	Ref<FileDialogue> FileDialogue::Create()
	{
	#ifdef HL_PLATFORM_WINDOWS
		return Ref<WindowsFileDialogue>::Create();
	#else
		return nullptr;
	#endif
	}
}