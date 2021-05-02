#pragma once

#include "FileDialogueFilter.h"

namespace highlo
{
	class FileDialogue
	{
	public:

		virtual ~FileDialogue() = default;

		virtual HLString SelectDirectory() = 0;
		virtual HLString SelectFile() = 0;
		virtual HLString SaveFile() = 0;

		void SetFilter(const FileDialogueFilter &filter);
		static Ref<FileDialogue> Create();

	protected:

		FileDialogueFilter m_Filter;
	};
}