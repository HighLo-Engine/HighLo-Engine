// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-03-01) initial release
//

#pragma once

namespace highlo
{
	class Translation
	{
	public:

		HLAPI Translation(const HLString &language, const HLString &languageCode);
		HLAPI virtual ~Translation();

		HLAPI void AddText(const HLString &key, const HLString &value);
		HLAPI HLString GetText(const HLString &key);
		HLAPI bool HasText(const HLString &key) const;

		HLAPI std::unordered_map<HLString, HLString> &GetAllTranslations() { return m_Translations; }
		HLAPI const std::unordered_map<HLString, HLString> &GetAllTranslations() const { return m_Translations; }

		HLAPI const HLString &GetLanguageText() const { return m_Language; }
		HLAPI const HLString &GetLanguageCode() const { return m_LanguageCode; }

	private:

		std::unordered_map<HLString, HLString> m_Translations;
		HLString m_Language;
		HLString m_LanguageCode;
	};
}

