#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Engine/Math/HLMath.h"
#include <map>

namespace highlo
{
#define HL_MATERIAL_TEXTURE_ALBEDO				0
#define HL_MATERIAL_TEXTURE_NORMAL				1
#define HL_MATERIAL_TEXTURE_METALLIC			2
#define HL_MATERIAL_TEXTURE_ROUGHNESS			3
#define HL_MATERIAL_TEXTURE_AMBIENT_OCCLUSION   4
#define HL_MATERIAL_TEXTURE_IRRADIANCE_MAP		5
#define HL_MATERIAL_TEXTURE_PREFILTER_MAP		6
#define HL_MATERIAL_TEXTURE_BRDF_MAP			7

	struct MaterialRenderProperties
	{
		glm::vec4 m_Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		float m_Roughness = 0.86f;
		float m_Metallic = 0.05f;
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

		HLAPI Ref<Texture> GetTexture(int32 type) const { return m_Textures.at(type); }
		HLAPI void SetTexture(int32 type, Ref<Texture> texture);
		HLAPI uint32 GetTextureCount() const { return (uint32)m_Textures.size(); }

		HLAPI virtual void Bind(bool update_properties = true);

	private:
		std::map<int32, Ref<Texture>> m_Textures;

	private:
		void CreateShaders();

		Ref<Shader> m_StaticShader;
		Ref<Shader> m_AnimatedShader;

		void ApplyNewProperties();

	private:
		Ref<UniformBuffer> m_MaterialDataBuffer = nullptr;
	};
}
