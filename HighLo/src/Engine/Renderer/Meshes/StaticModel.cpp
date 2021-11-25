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
		m_Materials = MaterialTable::Create((uint32)meshMaterials.size());

		for (uint32 i = 0; i < meshMaterials.size(); ++i)
			m_Materials->SetMaterial(i, MaterialAsset::Create(meshMaterials[i]));
	}

	StaticModel::StaticModel(Ref<MeshFile> &mesh, const std::vector<uint32> &subMeshIndices)
		: m_MeshFile(mesh)
	{
		SetSubmeshIndices(subMeshIndices);

		const auto &meshMaterials = mesh->GetMaterials();
		m_Materials = MaterialTable::Create((uint32)meshMaterials.size());

		for (uint32 i = 0; i < meshMaterials.size(); ++i)
			m_Materials->SetMaterial(i, MaterialAsset::Create(meshMaterials[i]));
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

