#include "HighLoPch.h"
#include "Environment.h"

#include "Renderer.h"

namespace highlo
{
	Environment::Environment(const Ref<Texture3D> &radianceMap, const Ref<Texture3D> &irradianceMap)
		: m_RadianceMap(radianceMap), m_IrradianceMap(irradianceMap) {}

	Ref<Environment> Environment::Create(const HLString &path)
	{
		return Renderer::CreateEnvironment(path);
	}
}