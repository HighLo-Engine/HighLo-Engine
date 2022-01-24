// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MaterialTable.h"

namespace highlo
{
	MaterialTable::MaterialTable(uint32 materialCount)
		: m_MaterialCount(materialCount)
	{
	}
	
	MaterialTable::MaterialTable(const Ref<MaterialTable> &other)
		: m_MaterialCount(other->m_MaterialCount)
	{
		const auto &materials = other->GetMaterials();
		for (auto [index, materialAsset] : materials)
		{
			SetMaterial(index, materialAsset);
		}
	}
	
	void MaterialTable::SetMaterial(uint32 index, const Ref<MaterialAsset> &material)
	{
		m_Materials[index] = material;
		if (index >= m_MaterialCount)
			m_MaterialCount = index + 1;
	}
	
	void MaterialTable::ClearMaterial(uint32 index)
	{
		HL_ASSERT(HasMaterial(index));
		m_Materials.erase(index);
		if (index >= m_MaterialCount)
			m_MaterialCount = index + 1;
	}
	
	Ref<MaterialAsset> MaterialTable::GetMaterial(uint32 index) const
	{
		HL_ASSERT(HasMaterial(index));
		return m_Materials.at(index);
	}
	
	void MaterialTable::Clear()
	{
		m_Materials.clear();
	}

	Ref<MaterialTable> MaterialTable::Create(uint32 materialCount)
	{
		return Ref<MaterialTable>::Create(materialCount);
	}
}

