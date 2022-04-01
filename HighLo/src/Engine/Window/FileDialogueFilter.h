// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/DataTypes/String.h"

namespace highlo
{
	class FileDialogueFilter
	{
	public:

		HLAPI void AddFilter(const HLString &name, const HLString &format);
		HLAPI bool HasFilters();
		HLAPI std::map<HLString, HLString> &GetFilters();

	private:

		std::map<HLString, HLString> m_Filters;
	};
}
