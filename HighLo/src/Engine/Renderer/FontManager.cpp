// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "FontManager.h"

namespace highlo
{
	void FontManager::Init()
	{
		// Load Default fonts
	}
	
	void FontManager::Shutdown()
	{
		// Replace every font with nullptr to decrement the refCount
		for (auto &[fontName, font] : m_Fonts)
			font = nullptr;

		m_Fonts.clear();
	}

	void FontManager::Add(const Ref<Font> &font)
	{
		HL_ASSERT(m_Fonts.find(font->GetName()) == m_Fonts.end());
		m_Fonts.insert({ font->GetName(), font });
	}

	void FontManager::Add(const HLString &fontName, const Ref<Font> &font)
	{
		HL_ASSERT(m_Fonts.find(fontName) == m_Fonts.end());
		m_Fonts.insert({ fontName, font });
	}

	void FontManager::Load(const FileSystemPath &path)
	{
		Ref<Font> font = Font::Create(path);
		Add(font->GetName(), font);
	}

	void FontManager::Load(const HLString &fontName, const FileSystemPath &path)
	{
		Ref<Font> font = Font::Create(path);
		Add(fontName, font);
	}

	Ref<Font> FontManager::GetFont(const HLString &fontName)
	{
		HL_ASSERT(m_Fonts.find(fontName) != m_Fonts.end());
		return m_Fonts[fontName];
	}

	bool FontManager::Has(const HLString &fontName) const
	{
		for (auto &[name, font] : m_Fonts)
		{
			if (name == fontName)
				return true;
		}

		return false;
	}
}

