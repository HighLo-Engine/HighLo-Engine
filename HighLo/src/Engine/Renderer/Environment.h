#pragma once

#include "Texture.h"

namespace highlo
{
	class Environment : public IsSharedReference
	{
	public:
		HLAPI Environment() = default;
		HLAPI Environment(const Ref<Texture3D>& skyboxTexture, const Ref<Texture3D> &radianceMap, const Ref<Texture3D> &irradianceMap, const Ref<Texture2D>& brdfMap);

		HLAPI inline const Ref<Texture3D> &GetSkyboxTexture() const { return m_SkyboxTexture; }
		HLAPI inline const Ref<Texture3D> &GetRadianceMap() const { return m_RadianceMap; }
		HLAPI inline const Ref<Texture3D> &GetIrradianceMap() const { return m_IrradianceMap; }
		HLAPI inline const Ref<Texture2D> &GetBRDFMap() const { return m_BRDFMap; }

		HLAPI static Ref<Environment> Create(const HLString &path);

	private:
		Ref<Texture3D> m_SkyboxTexture;
		Ref<Texture3D> m_RadianceMap;
		Ref<Texture3D> m_IrradianceMap;
		Ref<Texture2D> m_BRDFMap;
	};
}
