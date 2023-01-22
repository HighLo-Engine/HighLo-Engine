// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Assets/AssetMaterial.h"

namespace highlo
{
	class MaterialTable : public IsSharedReference
	{
	public:

		HLAPI MaterialTable(uint32 materialCount = 1);
		HLAPI MaterialTable(const Ref<MaterialTable> &other);
		HLAPI ~MaterialTable() = default;

		HLAPI bool HasMaterial(uint32 index) const { return m_Materials.find(index) != m_Materials.end(); }
		
		HLAPI void SetMaterial(uint32 index, const Ref<MaterialAsset> &material);
		HLAPI void ClearMaterial(uint32 index);
		HLAPI Ref<MaterialAsset> GetMaterial(uint32 index) const;
		HLAPI void Clear();

		HLAPI std::map<uint32, Ref<MaterialAsset>> &GetMaterials() { return m_Materials; }
		HLAPI const std::map<uint32, Ref<MaterialAsset>> &GetMaterials() const { return m_Materials; }

		HLAPI uint32 GetMaterialCount() const { return m_MaterialCount; }
		HLAPI void SetMaterialCount(uint32 materialCount) { m_MaterialCount = materialCount; }

		HLAPI bool HasAnyMaterial() const { return m_MaterialCount == 0; }

		HLAPI static Ref<MaterialTable> Create(uint32 materialCount = 1);

	private:

		std::map<uint32, Ref<MaterialAsset>> m_Materials;
		uint32 m_MaterialCount;
	};
}

