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

	static const HLString s_DiffuseMapUniform = "u_DiffuseTexture";
	static const HLString s_NormalMapUniform = "u_NormalTexture";
	static const HLString s_MetalnessMapUniform = "u_MetalnessTexture";
	static const HLString s_RoughnessMapUniform = "u_RoughnessTexture";

	Ref<MaterialAsset> MaterialAsset::Create()
	{
		return Ref<MaterialAsset>::Create();
	}

	Ref<MaterialAsset> MaterialAsset::Create(const Ref<Material> &material)
	{
		return Ref<MaterialAsset>::Create(material);
	}

	MaterialAsset::MaterialAsset()
	{
		m_Material = Material::Create(Renderer::GetShaderLibrary()->Get("DefaultShaderPBR"));

		// Set Default values
		SetDiffuseColor(glm::vec3(0.8f));
		SetEmission(0.0f);
		SetUsingNormalMap(false);
		SetMetalness(0.0f);
		SetRoughness(0.4f);

		SetDiffuseMap(Renderer::GetWhiteTexture());
		SetNormalMap(Renderer::GetWhiteTexture());
		SetMetalnessMap(Renderer::GetWhiteTexture());
		SetRoughnessMap(Renderer::GetWhiteTexture());
	}
	
	MaterialAsset::MaterialAsset(const Ref<Material> &material)
	{
		m_Material = Material::Copy(material);
	}
	
	MaterialAsset::~MaterialAsset()
	{
	}
	
	glm::vec3 &MaterialAsset::GetDiffuseColor()
	{
		return m_Material->GetVector3(s_DiffuseColorUniform);
	}
	
	void MaterialAsset::SetDiffuseColor(const glm::vec3 &value)
	{
		m_Material->Set(s_DiffuseColorUniform, value);
	}
	
	float &MaterialAsset::GetMetalness()
	{
		return m_Material->GetFloat(s_MetalnessUniform);
	}
	
	void MaterialAsset::SetMetalness(float value)
	{
		m_Material->Set(s_MetalnessUniform, value);
	}
	
	float &MaterialAsset::GetRoughness()
	{
		return m_Material->GetFloat(s_RoughnessUniform);
	}
	
	void MaterialAsset::SetRoughness(float value)
	{
		m_Material->Set(s_RoughnessUniform, value);
	}
	
	float &MaterialAsset::GetEmission()
	{
		return m_Material->GetFloat(s_EmissionUniform);
	}
	
	void MaterialAsset::SetEmission(float value)
	{
		m_Material->Set(s_EmissionUniform, value);
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
		return m_Material->GetTexture2D(s_NormalMapUniform);
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
		return m_Material->GetBool(s_UseNormalMapUniform);
	}
	
	void MaterialAsset::SetUsingNormalMap(bool value)
	{
		m_Material->Set(s_UseNormalMapUniform, value);
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
}

