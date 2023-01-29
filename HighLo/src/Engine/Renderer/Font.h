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
		int32 Codepoint;
		uint16 X;
		uint16 Y;
		uint16 Width;
		uint16 Height;
		int16 XOffset;
		int16 YOffset;
		int16 XAdvance;
		uint8 PageId;
	};

	struct FontKerning
	{
		int32 Codepoint1;
		int32 Codepoint2;
		int16 Amount;
	};

	enum class FontType
	{
		NONE = 0,
		BITMAP_FONT = 1,
		TRUE_TYPE_FONT = 2
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

		HLAPI virtual FileSystemPath &GetAssetPath() = 0;
		HLAPI virtual const FileSystemPath &GetAssetPath() const = 0;

		HLAPI virtual HLString &GetName() = 0;
		HLAPI virtual const HLString &GetName() const = 0;

		HLAPI static Ref<Font> Create(const FileSystemPath &path, FontType fontType = FontType::NONE);

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

