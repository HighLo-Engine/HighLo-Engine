// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DynamicModel.h"

#include "Engine/Assets/AssetMaterial.h"

namespace highlo
{
	DynamicModel::DynamicModel(Ref<MeshFile> &mesh)
		: m_MeshFile(mesh)
	{
		SetSubmeshIndices({});

		const auto &meshMaterials = mesh->GetMaterials();
		m_Materials = MaterialTable::Create((uint32)meshMaterials.size());

		for (uint32 i = 0; i < meshMaterials.size(); ++i)
			m_Materials->SetMaterial(i, MaterialAsset::Create(meshMaterials[i]));
	}

	DynamicModel::DynamicModel(Ref<MeshFile> &mesh, const std::vector<uint32> &submeshIndices)
		: m_MeshFile(mesh)
	{
		SetSubmeshIndices(submeshIndices);

		const auto &meshMaterials = m_MeshFile->GetMaterials();
		m_Materials = MaterialTable::Create((uint32)meshMaterials.size());

		for (uint32 i = 0; i < meshMaterials.size(); ++i)
			m_Materials->SetMaterial(i, MaterialAsset::Create(meshMaterials[i]));
	}

	DynamicModel::~DynamicModel()
	{
	}

	void DynamicModel::OnUpdate(Timestep ts)
	{
		if (m_IsAnimated)
		{
			if (m_AnimationPlaying)
			{
				m_WorldTime += ts;

				float ticksPerSecond = (float)(m_MeshFile->m_TicksPerSecond != 0 ? m_MeshFile->m_TicksPerSecond : 25.0f) * m_TimeMultiplier;
				m_AnimationTime += ts * ticksPerSecond;
				m_AnimationTime = fmod(m_AnimationTime, m_MeshFile->m_AnimationDuration);
			}

			m_MeshFile->ManipulateBoneTransform(m_AnimationTime);
		}
	}

	void DynamicModel::SetSubmeshIndices(const std::vector<uint32> &indices)
	{
		if (!indices.empty())
		{
			m_SubMeshIndices = indices;
		}
		else
		{
			const auto &submeshes = m_MeshFile->GetSubmeshes();
			m_SubMeshIndices.resize(submeshes.size());
			for (uint32 i = 0; i < submeshes.size(); ++i)
				m_SubMeshIndices[i] = i;
		}
	}
}

