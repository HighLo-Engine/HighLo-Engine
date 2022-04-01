#pragma once

#include "Translation.h"

namespace highlo
{
	class Translations
	{
	public:

		HLAPI static void Init();
		HLAPI static void Shutdown();

		HLAPI static void AddTranslation(const Translation &translation);
		HLAPI static Translation *GetTranslation(const HLString &languageCode);
		HLAPI static bool HasTranslation(const HLString &languageCode);

		HLAPI static std::vector<Translation> &GetAllTranslations();
		HLAPI static std::vector<HLString> GetAllLanguageCodes();
		HLAPI static std::vector<HLString> GetAllLanguages();

	private:

		static std::vector<Translation> s_Translations;
	};
}

