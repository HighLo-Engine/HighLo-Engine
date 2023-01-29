// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "TrueTypeFont.h"

namespace highlo
{
	TrueTypeFont::TrueTypeFont(const FileSystemPath &filePath)
		: m_AssetPath(filePath)
	{
		m_Name = filePath.Filename();
	}

	TrueTypeFont::~TrueTypeFont()
	{
	}
}

