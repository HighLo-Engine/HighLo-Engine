// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-21) initial release
//

#pragma once

#include "Engine/Assets/Asset.h"
#include "Engine/Core/FileSystemPath.h"
#include "Engine/Renderer/Texture.h"

namespace highlo
{
	struct MSDFData;

	class Font : public Asset
	{
	public:

		HLAPI Font(const FileSystemPath &path);
		HLAPI virtual ~Font();

		HLAPI static void Init();

		HLAPI Ref<Texture2D> &GetTextureAtlas() { return m_TextureAtlas; }
		HLAPI const Ref<Texture2D> &GetTextureAtlas() const { return m_TextureAtlas; }

		HLAPI static Ref<Font> GetDefaultFont();

		HLAPI static AssetType GetStaticType() { return AssetType::Font; }
		HLAPI virtual AssetType GetAssetType() const override { return GetStaticType(); }

		HLAPI static Ref<Font> Create(const FileSystemPath &path);

	private:

		FileSystemPath m_FilePath;
		MSDFData *m_MSDFData = nullptr;
		Ref<Texture2D> m_TextureAtlas;
	};
}

