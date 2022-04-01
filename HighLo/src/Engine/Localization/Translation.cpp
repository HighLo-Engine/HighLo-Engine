// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Translation.h"

namespace highlo
{
	Translation::Translation(const HLString &language, const HLString &languageCode)
		: m_Language(language), m_LanguageCode(languageCode)
	{
	}
	
	Translation::~Translation()
	{
	}

	void Translation::AddText(const HLString &key, const HLString &value)
	{
		m_Translations[key] = value;
	}

	HLString Translation::GetText(const HLString &key)
	{
		HL_ASSERT(m_Translations.find(key) != m_Translations.end(), "Could not find the key!");
		return m_Translations.at(key);
	}

	bool Translation::HasText(const HLString &key) const
	{
		return m_Translations.find(key) != m_Translations.end();
	}
}

