// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-01-29) initial release
//

#pragma once

#include "Engine/Renderer/Font.h"

namespace highlo
{
	class TrueTypeFont : public Font
	{
	public:

		TrueTypeFont(const FileSystemPath &filePath);
		virtual ~TrueTypeFont();

		virtual FileSystemPath &GetAssetPath() override { return m_AssetPath; }
		virtual const FileSystemPath &GetAssetPath() const override { return m_AssetPath; }

		virtual HLString &GetName() override { return m_Name; }
		virtual const HLString &GetName() const override { return m_Name; }

	private:

		FileSystemPath m_AssetPath;
		HLString m_Name;
	};
}

