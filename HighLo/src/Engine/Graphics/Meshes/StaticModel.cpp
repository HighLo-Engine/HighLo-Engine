// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "StaticModel.h"

#include "Engine/Assets/AssetMaterial.h"
#include "Engine/Assets/AssetManager.h"

namespace highlo
{
	StaticModel::StaticModel(const Ref<MeshFile> &meshSource)
		: m_MeshSource(meshSource)
	{
		// Generate a new asset handle
		Handle = {};

		SetSubmeshes({});

		const auto &meshMaterials = meshSource->GetMaterials();
		uint32 numMaterials = static_cast<uint32>(meshMaterials.size());
		m_Materials = Ref<MaterialTable>::Create(numMaterials);
		for (uint32 i = 0; i < numMaterials; i++)
		{
			AssetHandle materialHandle = AssetManager::Get()->CreateMemoryOnlyAsset<MaterialAsset>(meshMaterials[i]);
			m_Materials->SetMaterial(i, AssetManager::Get()->GetAsset<MaterialAsset>(materialHandle));
		}
	}

	StaticModel::StaticModel(const Ref<MeshFile> &meshSource, const std::vector<uint32> &submeshes)
		: m_MeshSource(meshSource)
	{
		// Generate a new asset handle
		Handle = {};

		SetSubmeshes(submeshes);

		const auto &meshMaterials = meshSource->GetMaterials();
		uint32 numMaterials = static_cast<uint32>(meshMaterials.size());
		m_Materials = Ref<MaterialTable>::Create(numMaterials);
		for (uint32 i = 0; i < numMaterials; i++)
		{
			AssetHandle materialHandle = AssetManager::Get()->CreateMemoryOnlyAsset<MaterialAsset>(meshMaterials[i]);
			m_Materials->SetMaterial(i, AssetManager::Get()->GetAsset<MaterialAsset>(materialHandle));
		}
	}

	StaticModel::StaticModel(const Ref<StaticModel> &other)
		: m_MeshSource(other->m_MeshSource), m_Materials(other->m_Materials)
	{
		SetSubmeshes(other->m_Submeshes);
	}

	StaticModel::~StaticModel()
	{
		Release();
	}

	void StaticModel::Release()
	{
		m_Submeshes.clear();
		m_Submeshes.shrink_to_fit();

		m_Materials->Clear();
	}

	void StaticModel::Invalidate()
	{
		Release();

		SetSubmeshes({});

		const auto &meshMaterials = m_MeshSource->GetMaterials();
		uint32 numMaterials = static_cast<uint32>(meshMaterials.size());
		m_Materials = Ref<MaterialTable>::Create(numMaterials);
		for (uint32 i = 0; i < numMaterials; i++)
		{
			AssetHandle materialHandle = AssetManager::Get()->CreateMemoryOnlyAsset<MaterialAsset>(meshMaterials[i]);
			m_Materials->SetMaterial(i, AssetManager::Get()->GetAsset<MaterialAsset>(materialHandle));
		}
	}

	void StaticModel::SetSubmeshes(const std::vector<uint32> &submeshes)
	{
		if (!submeshes.empty())
		{
			m_Submeshes = submeshes;
		}
		else
		{
			const auto &submeshes = m_MeshSource->GetSubmeshes();
			m_Submeshes.resize(submeshes.size());
			for (uint32 i = 0; i < submeshes.size(); i++)
				m_Submeshes[i] = i;
		}
	}
	void StaticModel::SetMeshAsset(const Ref<MeshFile> &meshAsset)
	{
		m_MeshSource = meshAsset;
		Invalidate();
	}
}

