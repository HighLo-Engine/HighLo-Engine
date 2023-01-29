// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "BitmapFont.h"

namespace highlo
{
	BitmapFont::BitmapFont(const FileSystemPath &filePath)
		: m_AssetPath(filePath)
	{
		m_Name = filePath.Filename();
	}

	BitmapFont::~BitmapFont()
	{
	}
}

