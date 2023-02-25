// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.4 (2023-01-29) Rewritten whole font implementation
//     - 1.3 (2021-10-05) Fixed warning in Release mode, because HL_ASSERT was used in release
//     - 1.2 (2021-09-26) Added MSDFData getter and moved GetDefaultFont to FontManager
//     - 1.1 (2021-09-22) Added Name getter
//     - 1.0 (2021-09-21) initial release
//

#pragma once

#include "Engine/Assets/Asset.h"
#include "Engine/Core/FileSystemPath.h"
#include "Engine/Graphics/Texture2D.h"

namespace highlo
{
	struct FontGlyph
	{
		// The representation of a 4-byte UTF8 character.
		int32 Codepoint;

		// The X position in pixels.
		uint16 X;

		// The Y position in pixels.
		uint16 Y;

		// The width of the box, the character is drawn in, in pixels.
		uint16 Width;

		// The height of the box, the character is drawn in, in pixels.
		uint16 Height;
		
		// The XOffset.
		float XOffset;

		// The YOffset.
		float YOffset;

		// The XAdvance.
		float XAdvance;

		// The page id, will not be used for now, but here for later refactorings
		// (at the moment we only support one texture atlas per font).
		uint8 PageId;
	};

	struct FontKerning
	{
		// The first UTF8 character.
		int32 Codepoint1;

		// The second UTF8 character.
		int32 Codepoint2;

		// The amount of kerning to apply.
		int32 Amount;
	};

	enum class FontType
	{
		NONE = 0,
		BITMAP_FONT = 1,
		TRUE_TYPE_FONT = 2
	};

	struct FontData
	{
		HLString Face;
		FontType Type = FontType::NONE;
		uint16 Size = 0;
		int32 LineHeight = 0;
		int32 Baseline = 0;
		int32 AtlasSizeX = 0;
		int32 AtlasSizeY = 0;
		float TabXAdvance = 0.0f;
		Ref<Texture2D> Atlas = nullptr;
		uint32 InternalDataSize = 0;
		void *InternalData = nullptr;
	};

	class Font : public Asset
	{
	public:

		HLAPI Font() = default;
		HLAPI virtual ~Font() = default;

		HLAPI static AssetType GetStaticType() { return AssetType::Font; }
		HLAPI virtual AssetType GetAssetType() const override { return GetStaticType(); }

		HLAPI std::vector<FontGlyph> &GetAllGlyphs() { return m_Glyphs; }
		HLAPI const std::vector<FontGlyph> &GetAllGlyphs() const { return m_Glyphs; }

		HLAPI std::vector<FontKerning> &GetAllKernings() { return m_Kernings; }
		HLAPI const std::vector<FontKerning> &GetAllKernings() const { return m_Kernings; }

		HLAPI FontGlyph &GetGlyphOfCodepoint(uint16 codepoint) { return m_Glyphs.at(codepoint); }

		HLAPI virtual bool VerifyAtlas(FontData *font, const HLString &text) = 0;

		HLAPI virtual FileSystemPath &GetAssetPath() = 0;
		HLAPI virtual const FileSystemPath &GetAssetPath() const = 0;

		HLAPI virtual HLString &GetName() = 0;
		HLAPI virtual const HLString &GetName() const = 0;

		HLAPI virtual FontType GetFontType() const = 0;
		HLAPI virtual FontData &GetFontDataOfCodepoint(uint16 codepoint) = 0;

		HLAPI virtual const Ref<Texture2D> &GetAtlas() const = 0;
		HLAPI virtual int32 GetAtlasSizeX() const = 0;
		HLAPI virtual int32 GetAtlasSizeY() const = 0;

		HLAPI virtual int32 GetLineHeight() const = 0;
		HLAPI virtual float GetTabXAdvance() const = 0;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="path"></param>
		/// <param name="size">For bitmap fonts it is not necessary, because the whole font file is only in one size. But TrueType fonts can be dynamic.</param>
		/// <param name="fontType"></param>
		/// <returns></returns>
		HLAPI static Ref<Font> Create(const FileSystemPath &path, uint16 size, FontType fontType = FontType::NONE);

	protected:

		std::vector<FontGlyph> m_Glyphs;
		std::vector<FontKerning> m_Kernings;
	};
}

#if 0

namespace highlo
{
	struct MSDFData;

	class Font : public Asset
	{
	public:

		HLAPI Font(const FileSystemPath &path);
		HLAPI virtual ~Font();

		HLAPI Ref<Texture2D> &GetTextureAtlas() { return m_TextureAtlas; }
		HLAPI const Ref<Texture2D> &GetTextureAtlas() const { return m_TextureAtlas; }

		HLAPI MSDFData *GetMSDFData() { return m_MSDFData; }
		HLAPI const MSDFData *GetMSDFData() const { return m_MSDFData; }

		HLAPI HLString &GetName() { return m_FontName; }
		HLAPI const HLString &GetName() const { return m_FontName; }

		HLAPI static AssetType GetStaticType() { return AssetType::Font; }
		HLAPI virtual AssetType GetAssetType() const override { return GetStaticType(); }

		HLAPI static Ref<Font> Create(const FileSystemPath &path);

	private:

		FileSystemPath m_FilePath;
		HLString m_FontName;
		MSDFData *m_MSDFData = nullptr;
		Ref<Texture2D> m_TextureAtlas;
	};
}

#endif

