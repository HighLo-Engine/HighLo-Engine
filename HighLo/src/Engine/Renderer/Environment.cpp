// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Environment.h"

#include "Renderer.h"

namespace highlo
{
	Environment::Environment(const Ref<Texture3D> &skyboxTexture, const Ref<Texture3D> &radianceMap, const Ref<Texture3D> &irradianceMap)
	{
		m_SkyboxTexture = skyboxTexture;
		m_RadianceMap = radianceMap;
		m_IrradianceMap = irradianceMap;
		m_BRDFMap = Renderer::GetBRDFLutTexture();
	}

	Environment::Environment(const FileSystemPath &filePath, const Ref<Texture3D> &skyboxTexture, const Ref<Texture3D> &radianceMap, const Ref<Texture3D> &irradianceMap)
	{
		m_FilePath = filePath;
		m_SkyboxTexture = skyboxTexture;
		m_RadianceMap = radianceMap;
		m_IrradianceMap = irradianceMap;
		m_BRDFMap = Renderer::GetBRDFLutTexture();
	}

	Ref<Environment> Environment::Create(const FileSystemPath &path)
	{
		return Renderer::CreateEnvironment(path);
	}
}

