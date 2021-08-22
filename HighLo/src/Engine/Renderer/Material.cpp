#include "HighLoPch.h"
#include "Material.h"

#include "Engine/Math/Math.h"
#include "Engine/Renderer/Renderer.h"

#include "Animation.h"

namespace highlo
{
	Ref<Material> Material::Create(MaterialProperties props)
	{
		return Ref<Material>(new Material(props));
	}

	Material::Material(MaterialProperties props)
		: Properties(props)
	{
		CreateShaders();
		ApplyNewProperties();

		// Setting default textures
		SetTexture(HL_MATERIAL_TEXTURE_ALBEDO, nullptr);
		SetTexture(HL_MATERIAL_TEXTURE_NORMAL, nullptr);
		SetTexture(HL_MATERIAL_TEXTURE_METALLIC, nullptr);
		SetTexture(HL_MATERIAL_TEXTURE_ROUGHNESS, nullptr);
		SetTexture(HL_MATERIAL_TEXTURE_AMBIENT_OCCLUSION, nullptr);
		SetTexture(HL_MATERIAL_TEXTURE_IRRADIANCE_MAP, nullptr);
		SetTexture(HL_MATERIAL_TEXTURE_PREFILTER_MAP, nullptr);
		SetTexture(HL_MATERIAL_TEXTURE_BRDF_MAP, nullptr);
	}

	void Material::CreateShaders()
	{
		static auto BoneTransformsBuffer = UniformBuffer::Create(
			"BoneTransformsBuffer",
			{
				UniformVariable("u_BoneTransforms", sizeof(glm::mat4) * HL_MAX_SKELETAL_BONES)
			},
			UniformBufferParentShader::VERTEX_SHADER,
			(uint32)HL_UB_SLOT::BONE_TRANSFORMS_BUFFER
		);

		m_StaticShader   = Renderer::GetShaderLibrary()->Get("DefaultShader");
		m_AnimatedShader = Renderer::GetShaderLibrary()->Get("DefaultAnimatedShader");
		m_StaticShaderPBR = Renderer::GetShaderLibrary()->Get("DefaultShaderPBR");
		m_AnimatedShaderPBR = Renderer::GetShaderLibrary()->Get("DefaultAnimatedShaderPBR");

		m_StaticShader->AddBuffer("VS_SceneBuffer", Shader::GetVSSceneUniformBuffer());
		m_StaticShaderPBR->AddBuffer("VS_SceneBuffer", Shader::GetVSSceneUniformBuffer());

		m_AnimatedShader->AddBuffer("VS_SceneBuffer", Shader::GetVSSceneUniformBuffer());
		m_AnimatedShader->AddBuffer("BoneTransformsBuffer", BoneTransformsBuffer);
		m_AnimatedShaderPBR->AddBuffer("VS_SceneBuffer", Shader::GetVSSceneUniformBuffer());
		m_AnimatedShaderPBR->AddBuffer("BoneTransformsBuffer", BoneTransformsBuffer);

		static auto VS_ObjectBuffer = UniformBuffer::Create(
			"VS_ObjectBuffer",
			{
				UniformVariable("u_Transform", sizeof(glm::mat4)),
			},
			UniformBufferParentShader::VERTEX_SHADER,
			(uint32)HL_UB_SLOT::VS_OBJECT_BUFFER
		);

		m_StaticShader->AddBuffer("VS_ObjectBuffer", VS_ObjectBuffer);
		m_StaticShaderPBR->AddBuffer("VS_ObjectBuffer", VS_ObjectBuffer);
		m_AnimatedShader->AddBuffer("VS_ObjectBuffer", VS_ObjectBuffer);
		m_AnimatedShaderPBR->AddBuffer("VS_ObjectBuffer", VS_ObjectBuffer);

		static auto MaterialDataBuffer = UniformBuffer::Create(
			"MaterialDataBuffer",
			{
				UniformVariable("u_Color", sizeof(glm::vec4)),
				UniformVariable("u_Roughness", sizeof(float)),
				UniformVariable("u_Metallic", sizeof(float)),
				UniformVariable("u_Padding001", sizeof(float)),
				UniformVariable("u_Padding002", sizeof(float)),
				UniformVariable("u_SpecularHighlight", sizeof(glm::vec3)),
				UniformVariable("u_Shininess", sizeof(float))
			},
			UniformBufferParentShader::PIXEL_SHADER,
			(uint32)HL_UB_SLOT::MATERIAL_DATA_BUFFER
		);

		m_MaterialDataBuffer = MaterialDataBuffer;

		m_StaticShader->AddBuffer("MaterialDataBuffer", MaterialDataBuffer);
		m_StaticShaderPBR->AddBuffer("MaterialDataBuffer", MaterialDataBuffer);
		m_AnimatedShader->AddBuffer("MaterialDataBuffer", MaterialDataBuffer);
		m_AnimatedShaderPBR->AddBuffer("MaterialDataBuffer", MaterialDataBuffer);
	}

	void Material::ApplyNewProperties()
	{
		m_MaterialDataBuffer->SetBufferValue(&Properties.m_RenderProperties);
		m_MaterialDataBuffer->UploadToShader();
	}

	void Material::SetTexture(int32 type, Ref<Texture> texture)
	{
		static Ref<Texture> s_DefaultAlbedo    = Texture2D::CreateFromColor({ 255, 255, 255 });
		static Ref<Texture> s_DefaultNormal    = Texture2D::CreateFromColor({ 128, 128, 255 });
		static Ref<Texture> s_DefaultMetallic  = Texture2D::CreateFromColor({ 255, 255, 255 });
		static Ref<Texture> s_DefaultRoughness = Texture2D::CreateFromColor({ 255, 255, 255 });
		static Ref<Texture> s_DefaultAO		   = Texture2D::CreateFromColor({ 255, 255, 255 });
		static Ref<Texture3D> blackCubeTexture = Renderer::GetBlackCubeTexture();

		if (!texture)
		{
			switch (type)
			{
				case HL_MATERIAL_TEXTURE_ALBEDO:				{ texture = s_DefaultAlbedo; break; }
				case HL_MATERIAL_TEXTURE_NORMAL:				{ texture = s_DefaultNormal; break; }
				case HL_MATERIAL_TEXTURE_METALLIC:				{ texture = s_DefaultMetallic; break; }
				case HL_MATERIAL_TEXTURE_ROUGHNESS:				{ texture = s_DefaultRoughness; break; }
				case HL_MATERIAL_TEXTURE_AMBIENT_OCCLUSION:		{ texture = s_DefaultAO; break; }
				case HL_MATERIAL_TEXTURE_IRRADIANCE_MAP:		{ texture = blackCubeTexture; break; }
				case HL_MATERIAL_TEXTURE_PREFILTER_MAP:			{ texture = blackCubeTexture; break; }
				case HL_MATERIAL_TEXTURE_BRDF_MAP:				{ texture = blackCubeTexture; break; }
				default: break;
			}
		}

		m_Textures[type] = texture;
	}

	void Material::Bind(bool update_properties)
	{
		if (update_properties)
			ApplyNewProperties();

		for (auto const& [slot, texture] : m_Textures)
			texture->Bind(slot);
	}

	uint32 Material::GetFlags() const
	{
		return m_MaterialFlags;
	}
	
	bool Material::GetFlag(MaterialFlag flag) const
	{
		return (uint32)flag & m_MaterialFlags;
	}
	
	void Material::SetFlag(MaterialFlag flag, bool value)
	{
		if (value)
		{
			m_MaterialFlags |= (uint32)flag;
		}
		else
		{
			m_MaterialFlags &= ~(uint32)flag;
		}
	}
}

