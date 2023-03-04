// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "BitmapFont.h"

namespace highlo
{
	BitmapFont::BitmapFont(const FileSystemPath &filePath, uint16 size)
		: m_AssetPath(filePath)
	{
		m_Name = filePath.Filename();
	}

	BitmapFont::~BitmapFont()
	{
	}
	
	bool BitmapFont::VerifyAtlas(FontData *font, const HLString &text)
	{
		// A bitmap font does not need verification
		return true;
	}

	FontData &BitmapFont::GetFontDataOfCodepoint(uint16 codepoint)
	{
		return m_TEMP;
	}

	const Ref<Texture2D> &BitmapFont::GetAtlas() const
	{
		return nullptr;
	}
	
	int32 BitmapFont::GetAtlasSizeX() const
	{
		return 0;
	}
	
	int32 BitmapFont::GetAtlasSizeY() const
	{
		return 0;
	}
	
	int32 BitmapFont::GetLineHeight() const
	{
		return 0;
	}
	
	float BitmapFont::GetTabXAdvance() const
	{
		return 0.0f;
	}
}

