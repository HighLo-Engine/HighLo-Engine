// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Texture.h"

namespace highlo
{
	class Environment : public IsSharedReference
	{
	public:

		HLAPI Environment() = default;
		HLAPI Environment(const FileSystemPath &filePath, const Ref<Texture3D> &skyboxTexture, const Ref<Texture3D> &radianceMap, const Ref<Texture3D> &irradianceMap, const Ref<Texture2D> &brdfMap);

		HLAPI inline const Ref<Texture3D> &GetSkyboxTexture() const { return m_SkyboxTexture; }
		HLAPI inline const Ref<Texture3D> &GetRadianceMap() const { return m_RadianceMap; }
		HLAPI inline const Ref<Texture3D> &GetIrradianceMap() const { return m_IrradianceMap; }
		HLAPI inline const Ref<Texture2D> &GetBRDFMap() const { return m_BRDFMap; }
		HLAPI inline const FileSystemPath &GetFilePath() const { return m_FilePath; }

		HLAPI static Ref<Environment> Create(const FileSystemPath &filePath);

	private:

		Ref<Texture3D> m_SkyboxTexture = nullptr;
		Ref<Texture3D> m_RadianceMap = nullptr;
		Ref<Texture3D> m_IrradianceMap = nullptr;
		Ref<Texture2D> m_BRDFMap = nullptr;
		FileSystemPath m_FilePath;
	};
}
