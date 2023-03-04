// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-01-29) initial release
//

#pragma once

#include "Engine/Core/FileSystemPath.h"

namespace highlo
{
	struct TrueTypeFontFace;

	struct BitmapFontLoaderResult
	{
		
	};

	struct TrueTypeFontLoaderResult
	{
		uint32 FontCount;
		TrueTypeFontFace *Fonts;

		uint64 BinarySize;
		Byte *FontBinary;
	};

	/// <summary>
	/// The FontLoader loads and saves engine specific binary fonts.
	/// </summary>
	class FontLoader
	{
	public:

		HLAPI static bool LoadTrueTypeFont(const FileSystemPath &filePath, TrueTypeFontLoaderResult *out_result);


	};
}

