// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.3 (2021-10-05) Fixed warning in Release mode, because HL_ASSERT was used in release
//     - 1.2 (2021-09-26) Added MSDFData getter and moved GetDefaultFont to FontManager
//     - 1.1 (2021-09-22) Added Name getter
//     - 1.0 (2021-09-21) initial release
//

#pragma once

#include "Engine/Assets/Asset.h"
#include "Engine/Core/FileSystemPath.h"
#include "Engine/Renderer/Texture2D.h"

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

