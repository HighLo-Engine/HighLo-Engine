// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-01-29) initial release
//

#pragma once

#include "Engine/Renderer/Font.h"
#include "Engine/Core/DataTypes/HashTable.h"

#include <stb_truetype.h>

namespace highlo
{
	struct TrueTypeFontFace
	{
		HLString Name;
	};

	/// <summary>
	/// This struct is going to be the internal_data of the general FontData, because it varies based on the font type.
	/// </summary>
	struct TrueTypeVariantData
	{
		std::vector<int32> Codepoints;
		float Scale = 0.0f;
	};

	struct TrueTypeFontData
	{
		uint16 ID;
		uint64 BinarySize;

		// The representation of each font data
		std::vector<FontData> SizeVariants;

		HLString Face;

		// The font binary loaded from the font file
		Byte *FontBinary;

		int32 Offset;
		int32 Index;

		// The internal font data of the third party library stb_truetype
		stbtt_fontinfo Info;
	};

	class TrueTypeFont : public Font
	{
	public:

		TrueTypeFont(const FileSystemPath &filePath, uint16 size);
		virtual ~TrueTypeFont();

		virtual FileSystemPath &GetAssetPath() override { return m_AssetPath; }
		virtual const FileSystemPath &GetAssetPath() const override { return m_AssetPath; }

		virtual HLString &GetName() override { return m_Name; }
		virtual const HLString &GetName() const override { return m_Name; }

		virtual bool VerifyAtlas(FontData *font, const HLString &text) override;

		virtual FontType GetFontType() const override { return FontType::TRUE_TYPE_FONT; }
		virtual FontData &GetFontDataOfCodepoint(uint16 codepoint) override;

		virtual const Ref<Texture2D> &GetAtlas() const override;
		virtual int32 GetAtlasSizeX() const override;
		virtual int32 GetAtlasSizeY() const override;

		virtual int32 GetLineHeight() const override;
		virtual float GetTabXAdvance() const override;

	private:

		bool LoadFont(uint32 default_size);
		bool SetupFontData(FontData *variant, uint16 font_size);
		void CleanupFontData(FontData *variant);

		bool CreateFontVariant(TrueTypeFontData data, uint16 size, const HLString &font_name, FontData *out_variant);
		bool RebuildFontVariantAtlas(TrueTypeFontData data, FontData *variant);
		bool VerifyFontSizeVariant(TrueTypeFontData data, FontData *variant, const HLString &text);

		const FontData *GetDefaultFontFace() const;

	private:

		FileSystemPath m_AssetPath;
		HLString m_Name;

		HashTable<uint16> m_FontIds; // Contains all font ids
		std::vector<TrueTypeFontData> m_Fonts;

		FontData m_TEMP;
	};
}

