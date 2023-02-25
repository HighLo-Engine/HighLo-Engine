// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#ifdef HL_PLATFORM_WINDOWS

#include <Windows.h>
#include <ShObjIdl.h>

#include "Engine/Window/FileDialogue.h"

namespace highlo
{
	class WindowsFileDialogue : public FileDialogue
	{
	public:

		virtual HLString SelectDirectory() override;
		virtual HLString SelectFile() override;
		virtual HLString SaveFile() override;

	private:

		HLString OpenFileDialogueInternal(FILEOPENDIALOGOPTIONS options, bool open);

	private:

		IFileOpenDialog *m_FileDialogue = nullptr;
	};
}

#endif // HL_PLATFORM_WINDOWS
