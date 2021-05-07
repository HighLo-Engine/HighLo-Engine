#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Engine/Math/HLMath.h"
#include <vector>

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
		HLAPI static Ref<Material> Create(MaterialProperties props = MaterialProperties());

		MaterialProperties Properties;

		/// <summary>
		/// Updates shaders with new data from m_Properties field.
		/// Must be called every time a property is changed and needed to be udpated.
		/// </summary>

		HLAPI void AddTexture(const Ref<Texture>& texture);
		HLAPI Ref<Texture> GetTexture(uint64 idx);
		HLAPI uint32 GetTextureCount() const { return (uint32)m_Textures.size(); }

		HLAPI virtual void Bind(bool update_properties = true);

	private:
		std::vector<Ref<Texture>> m_Textures;

	private:
		void CreateShaders();

		Ref<Shader> m_StaticShader;
		Ref<Shader> m_AnimatedShader;

		void ApplyNewProperties();

	private:
		Ref<UniformBuffer> m_MaterialDataBuffer = nullptr;
	};
}
