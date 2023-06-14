#pragma once

#include "Engine/Assets/Asset.h"
#include "Engine/Graphics/Material.h"

namespace highlo
{
	class MaterialAsset : public Asset
	{
	public:

		HLAPI MaterialAsset(bool transparent);
		HLAPI MaterialAsset(const Ref<Material> &material);
		HLAPI ~MaterialAsset();

		// Default values

		HLAPI glm::vec3 &GetDiffuseColor();
		HLAPI void SetDiffuseColor(const glm::vec3 &value);

		HLAPI float &GetMetalness();
		HLAPI void SetMetalness(float value);

		HLAPI float &GetRoughness();
		HLAPI void SetRoughness(float value);

		HLAPI float &GetEmission();
		HLAPI void SetEmission(float value);

		// Textures

		HLAPI Ref<Texture2D> GetDiffuseMap();
		HLAPI void SetDiffuseMap(const Ref<Texture2D> &texture);
		HLAPI void ClearDiffuseMap();

		HLAPI Ref<Texture2D> GetNormalMap();
		HLAPI void SetNormalMap(const Ref<Texture2D> &texture);
		HLAPI void ClearNormalMap();

		HLAPI bool IsUsingNormalMap();
		HLAPI void SetUsingNormalMap(bool value);

		HLAPI Ref<Texture2D> GetMetalnessMap();
		HLAPI void SetMetalnessMap(const Ref<Texture2D> &texture);
		HLAPI void CleartMetalnessMap();

		HLAPI Ref<Texture2D> GetRoughnessMap();
		HLAPI void SetRoughnessMap(const Ref<Texture2D> &texture);
		HLAPI void ClearRoughnessMap();
		
		HLAPI float &GetTransparency();
		HLAPI void SetTransparency(float transparency);

		HLAPI bool IsShadowCasting() const { return m_Material->GetFlag(MaterialFlag::DisableShadowCasting); }
		HLAPI void SetShadowCasting(bool castShadows) { m_Material->SetFlag(MaterialFlag::DisableShadowCasting, !castShadows); }

		HLAPI bool IsTransparent() const { return m_Transparent; }

		// Inherited via Asset
		HLAPI static AssetType GetStaticType() { return AssetType::Material; }
		HLAPI virtual AssetType GetAssetType() const override { return GetStaticType(); }

		HLAPI Ref<Material> &GetMaterial() { return m_Material; }
		HLAPI const Ref<Material> &GetMaterial() const { return m_Material; }

		HLAPI static Ref<MaterialAsset> Create(bool transparent);
		HLAPI static Ref<MaterialAsset> Create(const Ref<Material> &material);

	private:

		Ref<Material> m_Material = nullptr;
		bool m_Transparent = false;
	};
}

