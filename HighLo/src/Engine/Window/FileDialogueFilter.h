#pragma once

#include "Engine/Core/DataTypes/HLString.h"

namespace highlo
{
	class FileDialogueFilter
	{
	public:

		void AddFilter(const HLString &name, const HLString &format);
		bool HasFilters();
		std::map<HLString, HLString> &GetFilters();

	private:

		std::map<HLString, HLString> m_Filters;
	};
}