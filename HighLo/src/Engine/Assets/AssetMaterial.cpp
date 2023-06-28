#include "HighLoPch.h"
#include "AssetMaterial.h"

#include "Engine/Renderer/Renderer.h"

namespace highlo
{
	static const HLString s_DiffuseColorUniform = "u_MaterialUniforms.DiffuseColor";
	static const HLString s_UseNormalMapUniform = "u_MaterialUniforms.UseNormalMap";
	static const HLString s_MetalnessUniform = "u_MaterialUniforms.Metalness";
	static const HLString s_RoughnessUniform = "u_MaterialUniforms.Roughness";
	static const HLString s_EmissionUniform = "u_MaterialUniforms.Emission";
	static const HLString s_TransparencyUniform = "u_MaterialUniforms.Transparency";

	static const HLString s_DiffuseMapUniform = "u_DiffuseTexture";
	static const HLString s_NormalMapUniform = "u_NormalTexture";
	static const HLString s_MetalnessMapUniform = "u_MetalnessTexture";
	static const HLString s_RoughnessMapUniform = "u_RoughnessTexture";

	Ref<MaterialAsset> MaterialAsset::Create(bool transparent)
	{
		return Ref<MaterialAsset>::Create(transparent);
	}

	Ref<MaterialAsset> MaterialAsset::Create(const Ref<Material> &material)
	{
		return Ref<MaterialAsset>::Create(material);
	}

	MaterialAsset::MaterialAsset(bool transparent)
	{
		Handle = {}; // Assign a new UUID
		m_MaterialUniform = UniformBuffer::Create(sizeof(UniformBufferMaterial), 13, UniformLayout::GetMaterialLayout());

		m_Transparent = transparent;

		if (m_Transparent)
			// TODO: add the shader into the pipeline and fix all shader build errors
			m_Material = Material::Create(Renderer::GetShaderLibrary()->Get("HighLoPBR_Transparent"));
		else
			m_Material = Material::Create(Renderer::GetShaderLibrary()->Get("HighLoPBR"));

		// Set Default values
		SetDiffuseColor(glm::vec3(1.0f));
		SetEmission(0.0f);
		SetUsingNormalMap(false);
		SetMetalness(0.0f);
		SetRoughness(0.4f);
		SetTransparency(0.0f);

		SetDiffuseMap(Renderer::GetWhiteTexture());
		SetNormalMap(Renderer::GetWhiteTexture());
		SetMetalnessMap(Renderer::GetWhiteTexture());
		SetRoughnessMap(Renderer::GetWhiteTexture());
	}
	
	MaterialAsset::MaterialAsset(const Ref<Material> &material)
	{
		Handle = {}; // Assign a new UUID
		m_Material = Material::Copy(material);
		m_MaterialUniform = UniformBuffer::Create(sizeof(UniformBufferMaterial), 13, UniformLayout::GetMaterialLayout());

		// Set Default values
		SetDiffuseColor(glm::vec3(0.8f));
		SetEmission(0.0f);
		SetUsingNormalMap(false);
		SetMetalness(0.0f);
		SetRoughness(0.4f);
		SetTransparency(0.0f);

		SetDiffuseMap(Renderer::GetWhiteTexture());
		SetNormalMap(Renderer::GetWhiteTexture());
		SetMetalnessMap(Renderer::GetWhiteTexture());
		SetRoughnessMap(Renderer::GetWhiteTexture());
	}
	
	MaterialAsset::~MaterialAsset()
	{
		m_MaterialUniform = nullptr;
	}

	void MaterialAsset::UploadToGPU()
	{
		m_MaterialUniform->SetData(&m_MaterialProperties, sizeof(UniformBufferMaterial));
	}
	
	glm::vec3 &MaterialAsset::GetDiffuseColor()
	{
		return m_MaterialProperties.DiffuseColor;
	}
	
	void MaterialAsset::SetDiffuseColor(const glm::vec3 &value)
	{
		m_MaterialProperties.DiffuseColor = value;
		UploadToGPU();
	}
	
	float &MaterialAsset::GetMetalness()
	{
		return m_MaterialProperties.Metalness;
	}
	
	void MaterialAsset::SetMetalness(float value)
	{
		m_MaterialProperties.Metalness = value;
		UploadToGPU();
	}
	
	float &MaterialAsset::GetRoughness()
	{
		return m_MaterialProperties.Roughness;
	}
	
	void MaterialAsset::SetRoughness(float value)
	{
		m_MaterialProperties.Roughness = value;
		UploadToGPU();
	}
	
	float &MaterialAsset::GetEmission()
	{
		return m_MaterialProperties.Emission;
	}
	
	void MaterialAsset::SetEmission(float value)
	{
		m_MaterialProperties.Emission = value;
		UploadToGPU();
	}
	
	Ref<Texture2D> MaterialAsset::GetDiffuseMap()
	{
		return m_Material->GetTexture2D(s_DiffuseMapUniform);
	}
	
	void MaterialAsset::SetDiffuseMap(const Ref<Texture2D> &texture)
	{
		m_Material->Set(s_DiffuseMapUniform, texture);
	}
	
	void MaterialAsset::ClearDiffuseMap()
	{
		m_Material->Set(s_DiffuseMapUniform, Renderer::GetWhiteTexture());
	}
	
	Ref<Texture2D> MaterialAsset::GetNormalMap()
	{
		return m_Material->TryGetTexture2D(s_NormalMapUniform);
	}
	
	void MaterialAsset::SetNormalMap(const Ref<Texture2D> &texture)
	{
		m_Material->Set(s_NormalMapUniform, texture);
	}
	
	void MaterialAsset::ClearNormalMap()
	{
		m_Material->Set(s_NormalMapUniform, Renderer::GetWhiteTexture());
	}
	
	bool MaterialAsset::IsUsingNormalMap()
	{
		return (bool)m_MaterialProperties.UseNormalMap;
	}
	
	void MaterialAsset::SetUsingNormalMap(bool value)
	{
		m_MaterialProperties.UseNormalMap = value;
		UploadToGPU();
	}
	
	Ref<Texture2D> MaterialAsset::GetMetalnessMap()
	{
		return m_Material->GetTexture2D(s_MetalnessMapUniform);
	}
	
	void MaterialAsset::SetMetalnessMap(const Ref<Texture2D> &texture)
	{
		m_Material->Set(s_MetalnessMapUniform, texture);
	}
	
	void MaterialAsset::CleartMetalnessMap()
	{
		m_Material->Set(s_MetalnessMapUniform, Renderer::GetWhiteTexture());
	}
	
	Ref<Texture2D> MaterialAsset::GetRoughnessMap()
	{
		return m_Material->GetTexture2D(s_RoughnessMapUniform);
	}

	void MaterialAsset::SetRoughnessMap(const Ref<Texture2D> &texture)
	{
		m_Material->Set(s_RoughnessMapUniform, texture);
	}
	
	void MaterialAsset::ClearRoughnessMap()
	{
		m_Material->Set(s_RoughnessMapUniform, Renderer::GetWhiteTexture());
	}
	
	float &MaterialAsset::GetTransparency()
	{
		return m_MaterialProperties.Transparency;
	}

	void MaterialAsset::SetTransparency(float transparency)
	{
		m_MaterialProperties.Transparency = transparency;
		UploadToGPU();
	}
}

