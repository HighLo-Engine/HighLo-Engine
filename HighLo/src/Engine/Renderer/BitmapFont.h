// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-01-29) initial release
//

#pragma once

#include "Engine/Renderer/Font.h"

namespace highlo
{
	class BitmapFont : public Font
	{
	public:

		BitmapFont(const FileSystemPath &filePath, uint16 size);
		virtual ~BitmapFont();

		virtual FileSystemPath &GetAssetPath() override { return m_AssetPath; }
		virtual const FileSystemPath &GetAssetPath() const override { return m_AssetPath; }

		virtual HLString &GetName() override { return m_Name; }
		virtual const HLString &GetName() const override { return m_Name; }

		virtual bool VerifyAtlas(FontData *font, const HLString &text) override;

		virtual FontType GetFontType() const override { return FontType::BITMAP_FONT; }
		virtual FontData &GetFontDataOfCodepoint(uint16 codepoint) override;

		virtual const Ref<Texture2D> &GetAtlas() const override;
		virtual int32 GetAtlasSizeX() const override;
		virtual int32 GetAtlasSizeY() const override;

		virtual int32 GetLineHeight() const override;
		virtual float GetTabXAdvance() const override;

	private:

		FileSystemPath m_AssetPath;
		HLString m_Name;

		FontData m_TEMP;
	};
}

