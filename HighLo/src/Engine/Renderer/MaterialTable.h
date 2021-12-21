// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Material.h"

namespace highlo
{
	class MaterialTable : public IsSharedReference
	{
	public:

		HLAPI MaterialTable(uint32 materialCount = 1);
		HLAPI MaterialTable(const Ref<MaterialTable> &other);
		HLAPI ~MaterialTable() = default;

		HLAPI bool HasMaterial(uint32 index) const { return m_Materials.find(index) != m_Materials.end(); }
		
		// TODO: Change Ref<Material> to be a Asset material when we have assets
		HLAPI void SetMaterial(uint32 index, const Ref<Material> &material);
		HLAPI void ClearMaterial(uint32 index);
		HLAPI Ref<Material> GetMaterial(uint32 index) const;
		HLAPI void Clear();

		HLAPI std::map<uint32, Ref<Material>> &GetMaterials() { return m_Materials; }
		HLAPI const std::map<uint32, Ref<Material>> &GetMaterials() const { return m_Materials; }

		HLAPI uint32 GetMaterialCount() const { return m_MaterialCount; }
		HLAPI void SetMaterialCount(uint32 materialCount) { m_MaterialCount = materialCount; }

		HLAPI static Ref<MaterialTable> Create(uint32 materialCount = 1);

	private:

		// TODO: change the Material to be a MaterialAsset, if we have an asset implementation
		std::map<uint32, Ref<Material>> m_Materials;
		uint32 m_MaterialCount;
	};
}

