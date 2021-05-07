#pragma once

#include "Texture.h"

namespace highlo
{
	class Environment : public IsSharedReference
	{
	public:

		HLAPI Environment() = default;
		HLAPI Environment(const Ref<Texture3D> &radianceMap, const Ref<Texture3D> &irradianceMap);

		HLAPI inline const Ref<Texture3D> &GetRadianceMap() const { return m_RadianceMap; }
		HLAPI inline const Ref<Texture3D> &GetIrradianceMap() const { return m_IrradianceMap; }

		HLAPI static Ref<Environment> Create(const HLString &path);

	private:

		Ref<Texture3D> m_RadianceMap;
		Ref<Texture3D> m_IrradianceMap;
	};
}
