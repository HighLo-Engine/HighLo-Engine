#pragma once
#include "Shader.h"

namespace highlo
{
	struct MaterialRenderProperties
	{
		float m_Roughness = 0.0f;
	};

	struct MaterialPhysicalProperties
	{
		float m_Weight = 1.0f;
	};

	struct MaterialProperties
	{
		MaterialRenderProperties    m_RenderProperties;
		MaterialPhysicalProperties  m_PhysicalProperties;
	};

	class Material : public IsSharedReference
	{
		friend class CoreRenderer;

		Material(MaterialProperties props = MaterialProperties());

	public:
		static Ref<Material> Create(MaterialProperties props = MaterialProperties());

		MaterialProperties m_Properties;

	private:
		void CreateShaders();

		Ref<Shader> m_StaticShader;
		Ref<Shader> m_AnimatedShader;
	};
}
