// Copyright (c) 2021 Albert Slepak and Can Karka. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "FileDialogueFilter.h"

namespace highlo
{
	class FileDialogue : public IsSharedReference
	{
	public:

		HLAPI virtual ~FileDialogue() = default;

		HLAPI virtual HLString SelectDirectory() = 0;
		HLAPI virtual HLString SelectFile() = 0;
		HLAPI virtual HLString SaveFile() = 0;

		HLAPI void SetFilter(const FileDialogueFilter &filter);
		HLAPI static Ref<FileDialogue> Create();

	protected:

		FileDialogueFilter m_Filter;
	};
}