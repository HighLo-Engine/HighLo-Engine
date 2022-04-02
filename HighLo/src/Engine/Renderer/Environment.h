// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Graphics/Texture2D.h"
#include "Engine/Graphics/Texture3D.h"

namespace highlo
{
	enum class EnvironmentType
	{
		None	 = 0,
		Preetham = 1,
		HDR		 = 2
	};

	class Environment : public Asset
	{
	public:

		HLAPI Environment() = default;
		HLAPI Environment(const Ref<Texture3D> &skyboxTexture, const Ref<Texture3D> &radianceMap, const Ref<Texture3D> &irradianceMap);
		HLAPI Environment(const FileSystemPath &filePath, const Ref<Texture3D> &skyboxTexture, const Ref<Texture3D> &radianceMap, const Ref<Texture3D> &irradianceMap);

		HLAPI inline const Ref<Texture3D> &GetSkyboxTexture() const { return m_SkyboxTexture; }
		HLAPI inline const Ref<Texture3D> &GetRadianceMap() const { return m_RadianceMap; }
		HLAPI inline const Ref<Texture3D> &GetIrradianceMap() const { return m_IrradianceMap; }
		HLAPI inline const Ref<Texture2D> &GetBRDFMap() const { return m_BRDFMap; }
		HLAPI inline const FileSystemPath &GetFilePath() const { return m_FilePath; }

		HLAPI inline void SetRadianceMap(const Ref<Texture3D> &texture) { m_RadianceMap = texture; }
		HLAPI inline void SetIrradianceMap(const Ref<Texture3D> &texture) { m_IrradianceMap = texture; }

		HLAPI inline EnvironmentType GetType() const { return m_Type; }

		// Inherited from Asset
		HLAPI static AssetType GetStaticType() { return AssetType::EnvMap; }
		HLAPI virtual AssetType GetAssetType() const { return GetStaticType(); }

		HLAPI static Ref<Environment> Create(const FileSystemPath &filePath);

	private:

		Ref<Texture3D> m_SkyboxTexture = nullptr;		// unfiltered radiance map
		Ref<Texture3D> m_RadianceMap = nullptr;			// filtered radiance map
		Ref<Texture3D> m_IrradianceMap = nullptr;		// irradiance map
		Ref<Texture2D> m_BRDFMap = nullptr;				// BRDF map
		FileSystemPath m_FilePath;						// FilePath if HDR texture was loaded, NULL if preetham sky was created
		EnvironmentType m_Type = EnvironmentType::None; // Determines the type of the current environment instance
	};
}
