// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include <map>

#include "Shader.h"
#include "Texture.h"
#include "Engine/Math/Math.h"

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

	enum class MaterialFlag
	{
		None		= HL_BIT(0),
		DepthTest	= HL_BIT(1),
		Blend		= HL_BIT(2),
		TwoSided	= HL_BIT(3),
	};

	struct MaterialRenderProperties
	{
		glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		float Roughness = 0.86f;
		float Metallic = 0.05f;
		float UnkPadding0;
		float UnkPadding1;
		glm::vec3 SpecularHighlight = glm::vec3(1.0f, 1.0f, 1.0f);
		float Shininess = 0.25f;
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

	public:

		HLAPI Material(MaterialProperties props = MaterialProperties());

		HLAPI static Ref<Material> Create(MaterialProperties props = MaterialProperties());

		MaterialProperties Properties;
		bool IsUsingPBR = true;

		HLAPI Ref<Texture> GetTexture(int32 type) const { return m_Textures.at(type); }
		HLAPI void SetTexture(int32 type, Ref<Texture> texture);
		HLAPI uint32 GetTextureCount() const { return (uint32)m_Textures.size(); }

		HLAPI virtual void Bind(bool update_properties = true);

		HLAPI uint32 GetFlags() const;
		HLAPI bool GetFlag(MaterialFlag flag) const;
		HLAPI void SetFlag(MaterialFlag flag, bool value = true);

	private:
		std::map<int32, Ref<Texture>> m_Textures;

	private:
		void CreateShaders();

		Ref<Shader> m_StaticShader;
		Ref<Shader> m_AnimatedShader;
		Ref<Shader> m_StaticShaderPBR;
		Ref<Shader> m_AnimatedShaderPBR;

		void ApplyNewProperties();

	private:

		Ref<UniformBuffer> m_MaterialDataBuffer = nullptr;
		uint32 m_MaterialFlags = 0;
	};
}
