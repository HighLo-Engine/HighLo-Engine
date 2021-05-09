#include "HighLoPch.h"
#include "Material.h"

#include "Engine/Math/HLMath.h"
#include "Engine/Renderer/Renderer.h"

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
		constexpr uint32 MAX_BONES = 150;
		static auto BoneTransformsBuffer = UniformBuffer::Create(
			"BoneTransformsBuffer",
			{
				UniformVariable("u_BoneTransforms", sizeof(glm::mat4) * MAX_BONES)
			},
			UniformBufferParentShader::VERTEX_SHADER,
			(uint32)HL_UB_SLOT::BONE_TRANSFORMS_BUFFER
		);

		m_StaticShader   = Renderer::GetShaderLibrary()->Get("DefaultShader");
		m_AnimatedShader = Renderer::GetShaderLibrary()->Get("DefaultAnimatedShader");

		m_StaticShader->AddBuffer("VS_SceneBuffer", Shader::GetVSSceneUniformBuffer());
		m_AnimatedShader->AddBuffer("VS_SceneBuffer", Shader::GetVSSceneUniformBuffer());
		m_AnimatedShader->AddBuffer("BoneTransformsBuffer", BoneTransformsBuffer);

		static auto VS_ObjectBuffer = UniformBuffer::Create(
			"VS_ObjectBuffer",
			{
				UniformVariable("u_Transform", sizeof(glm::mat4)),
			},
			UniformBufferParentShader::VERTEX_SHADER,
			(uint32)HL_UB_SLOT::VS_OBJECT_BUFFER
		);

		m_StaticShader->AddBuffer("VS_ObjectBuffer", VS_ObjectBuffer);
		m_AnimatedShader->AddBuffer("VS_ObjectBuffer", VS_ObjectBuffer);

		static auto MaterialDataBuffer = UniformBuffer::Create(
			"MaterialDataBuffer",
			{
				UniformVariable("u_Color", sizeof(glm::vec4)),
				UniformVariable("u_Roughness", sizeof(float)),
				UniformVariable("u_Metallic", sizeof(float)),
			},
			UniformBufferParentShader::PIXEL_SHADER,
			(uint32)HL_UB_SLOT::MATERIAL_DATA_BUFFER
		);

		m_MaterialDataBuffer = MaterialDataBuffer;

		m_StaticShader->AddBuffer("MaterialDataBuffer", MaterialDataBuffer);
		m_AnimatedShader->AddBuffer("MaterialDataBuffer", MaterialDataBuffer);
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

		static uint32 s_BlackTextureData[6] = { 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000, 0xff000000 };
		static Ref<Texture> s_BlackCubeTexture = Texture3D::Create(ImageFormat::RGBA, 1, 1, &s_BlackTextureData).As<Texture3D>();

		if (!texture)
		{
			switch (type)
			{
				case HL_MATERIAL_TEXTURE_ALBEDO:				{ texture = s_DefaultAlbedo; break; }
				case HL_MATERIAL_TEXTURE_NORMAL:				{ texture = s_DefaultNormal; break; }
				case HL_MATERIAL_TEXTURE_METALLIC:				{ texture = s_DefaultMetallic; break; }
				case HL_MATERIAL_TEXTURE_ROUGHNESS:				{ texture = s_DefaultRoughness; break; }
				case HL_MATERIAL_TEXTURE_AMBIENT_OCCLUSION:		{ texture = s_DefaultAO; break; }
				case HL_MATERIAL_TEXTURE_IRRADIANCE_MAP:		{ texture = s_BlackCubeTexture; break; }
				case HL_MATERIAL_TEXTURE_PREFILTER_MAP:			{ texture = s_BlackCubeTexture; break; }
				case HL_MATERIAL_TEXTURE_BRDF_MAP:				{ texture = s_BlackCubeTexture; break; }
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
}
