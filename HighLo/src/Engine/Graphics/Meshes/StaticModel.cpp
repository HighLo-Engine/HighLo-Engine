// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "StaticModel.h"

#include "Engine/Assets/AssetMaterial.h"

namespace highlo
{
	StaticModel::StaticModel(Ref<MeshFile> &mesh)
		: m_MeshFile(mesh)
	{
		SetSubmeshIndices({});

		const auto &meshMaterials = mesh->GetMaterials();

		// it does not matter, if meshMaterials.size() == 0,
		// because if we add a new material it will be automatically resize to the new amount of materials
		m_Materials = MaterialTable::Create((uint32)meshMaterials.size());

		if (meshMaterials.size() == 0)
		{
			// Make sure, that the model always has a default material
			m_Materials->SetMaterial(0, MaterialAsset::Create(false)); // TODO: forward castShadows from constructor parameter
		}
		else
		{
			for (uint32 i = 0; i < meshMaterials.size(); ++i)
				m_Materials->SetMaterial(i, MaterialAsset::Create(meshMaterials[i]));
		}
	}

	StaticModel::StaticModel(Ref<MeshFile> &mesh, const std::vector<uint32> &subMeshIndices)
		: m_MeshFile(mesh)
	{
		SetSubmeshIndices(subMeshIndices);

		const auto &meshMaterials = mesh->GetMaterials();

		// Same as above here
		m_Materials = MaterialTable::Create((uint32)meshMaterials.size());

		if (meshMaterials.size() == 0)
		{
			m_Materials->SetMaterial(0, MaterialAsset::Create(false));
		}
		else
		{
			for (uint32 i = 0; i < meshMaterials.size(); ++i)
				m_Materials->SetMaterial(i, MaterialAsset::Create(meshMaterials[i]));
		}
	}

	StaticModel::~StaticModel()
	{
	}

	void StaticModel::SetSubmeshIndices(const std::vector<uint32> &submeshIndices)
	{
		if (!submeshIndices.empty())
		{
			m_SubMeshIndices = submeshIndices;
		}
		else
		{
			const auto &submeshes = m_MeshFile->GetSubmeshes();
			m_SubMeshIndices.resize(submeshes.size());
			
			for (uint32 i = 0; i < m_SubMeshIndices.size(); ++i)
				m_SubMeshIndices[i] = i;
		}
	}
}

