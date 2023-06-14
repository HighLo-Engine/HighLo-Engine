// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "FileDialogueFilter.h"

namespace highlo
{
	void FileDialogueFilter::AddFilter(const HLString &name, const HLString &format)
	{
		m_Filters.insert({ name, format });
	}

	bool FileDialogueFilter::HasFilters()
		{
		return m_Filters.size();
		}

	std::map<HLString, HLString> &FileDialogueFilter::GetFilters()
		{
		return m_Filters;
		}

}
