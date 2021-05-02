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