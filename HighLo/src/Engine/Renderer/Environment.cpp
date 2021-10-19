// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Environment.h"

#include "Renderer.h"

namespace highlo
{
	Environment::Environment(const Ref<Texture3D> &skyboxTexture, const Ref<Texture3D> &radianceMap, const Ref<Texture3D> &irradianceMap, const Ref<Texture2D> &brdfMap)
		: m_SkyboxTexture(skyboxTexture), m_RadianceMap(radianceMap), m_IrradianceMap(irradianceMap), m_BRDFMap(brdfMap) {}

	Ref<Environment> Environment::Create(const HLString &path)
	{
		return Renderer::CreateEnvironment(path);
	}
}