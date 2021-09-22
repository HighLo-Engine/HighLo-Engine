// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-22) initial release
//

#pragma once

#include "Font.h"

namespace highlo
{
	class FontManager : public Singleton<FontManager>
	{
	public:

		HLAPI void Init();
		HLAPI void Shutdown();

		HLAPI void Add(const Ref<Font> &font);
		HLAPI void Add(const HLString &fontName, const Ref<Font> &font);

		HLAPI void Load(const FileSystemPath &path);
		HLAPI void Load(const HLString &fontName, const FileSystemPath &path);

		HLAPI Ref<Font> GetFont(const HLString &fontName);
		HLAPI bool Has(const HLString &fontName) const;

	private:

		std::unordered_map<HLString, Ref<Font>> m_Fonts;
	};
}

