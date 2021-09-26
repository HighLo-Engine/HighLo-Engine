// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-09-26) Moved the DefaultFont function from Font to here and added GetFonts for all fonts
//     - 1.0 (2021-09-22) initial release
//

#pragma once

#include "Font.h"
#include "Engine/Core/Singleton.h"

namespace highlo
{
	class FontManager : public Singleton<FontManager>
	{
	public:

		HLAPI void Init();
		HLAPI void Shutdown();

		HLAPI void AddFont(const Ref<Font> &font);
		HLAPI void AddFont(const HLString &fontName, const Ref<Font> &font);

		HLAPI void LoadFont(const FileSystemPath &path);
		HLAPI void LoadFont(const HLString &fontName, const FileSystemPath &path);

		HLAPI std::vector<Ref<Font>> GetFonts();
		HLAPI Ref<Font> GetFont(const HLString &fontName);
		HLAPI Ref<Font> GetDefaultFont();
		HLAPI bool HasFont(const HLString &fontName) const;

	private:

		std::unordered_map<HLString, Ref<Font>> m_Fonts;
	};
}

