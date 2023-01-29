// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "FontManager.h"

namespace highlo
{
	void FontManager::Init()
	{
		// Load Default fonts
		AddFont("BarlowSemiCondensed-Black", Font::Create("assets/fonts/BarlowSemiCondensedFontFamily/BarlowSemiCondensed-Black.ttf", FontType::TRUE_TYPE_FONT));
		//AddFont("OpenSans", Font::Create("assets/fonts/opensans/OpenSans-Bold.ttf", FontType::TRUE_TYPE_FONT));
	}
	
	void FontManager::Shutdown()
	{
		// Replace every font with nullptr to decrement the refCount
		for (auto &[fontName, font] : m_Fonts)
			font = nullptr;

		m_Fonts.clear();
	}

	void FontManager::AddFont(const Ref<Font> &font)
	{
		HL_ASSERT(m_Fonts.find(font->GetName()) == m_Fonts.end());
		m_Fonts.insert({ font->GetName(), font });
	}

	void FontManager::AddFont(const HLString &fontName, const Ref<Font> &font)
	{
		HL_ASSERT(m_Fonts.find(fontName) == m_Fonts.end());
		m_Fonts.insert({ fontName, font });
	}

	void FontManager::LoadFont(const FileSystemPath &path)
	{
		Ref<Font> font = Font::Create(path);
		AddFont(font->GetName(), font);
	}

	void FontManager::LoadFont(const HLString &fontName, const FileSystemPath &path)
	{
		Ref<Font> font = Font::Create(path);
		AddFont(fontName, font);
	}

	std::vector<Ref<Font>> FontManager::GetFonts()
	{
		std::vector<Ref<Font>> fonts;

		for (auto &[fontName, font] : m_Fonts)
			fonts.push_back(font);

		return fonts;
	}

	Ref<Font> FontManager::GetFont(const HLString &fontName)
	{
		HL_ASSERT(m_Fonts.find(fontName) != m_Fonts.end());
		return m_Fonts[fontName];
	}

	Ref<Font> FontManager::GetDefaultFont()
	{
		auto &fonts = GetFonts();
		HL_ASSERT(fonts.size() >= 1);
		return fonts[0];
	}

	bool FontManager::HasFont(const HLString &fontName) const
	{
		for (auto &[name, font] : m_Fonts)
		{
			if (name == fontName)
				return true;
		}

		return false;
	}
}

