#pragma once

#include "Shader.h"
#include "Engine/Math/HLMath.h"

namespace highlo
{
	struct MaterialRenderProperties
	{
		glm::vec4 m_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
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

		MaterialProperties Properties;

		/// <summary>
		/// Updates shaders with new data from m_Properties field.
		/// Must be called every time a property is changed and needed to be udpated.
		/// </summary>
		void ApplyNewProperties();

	private:
		void CreateShaders();

		Ref<Shader> m_StaticShader;
		Ref<Shader> m_AnimatedShader;

	private:
		Ref<UniformBuffer> m_MaterialDataBuffer = nullptr;
	};
}
