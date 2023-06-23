// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "DynamicModel.h"

#include "Engine/Assets/AssetMaterial.h"
#include "Engine/Assets/AssetManager.h"

namespace highlo
{
	DynamicModel::DynamicModel(const Ref<MeshFile> &meshSource)
		: m_MeshSource(meshSource)
	{
		// Generate a new asset handle
		Handle = {};

		SetSubmeshes({});

		const auto &meshMaterials = meshSource->GetMaterials();
		m_Materials = Ref<MaterialTable>::Create((uint32)meshMaterials.size());
		for (size_t i = 0; i < meshMaterials.size(); i++)
		{
			AssetHandle materialHandle = AssetManager::Get()->CreateMemoryOnlyAsset<MaterialAsset>(meshMaterials[i]);
			m_Materials->SetMaterial((uint32)i, AssetManager::Get()->GetAsset<MaterialAsset>(materialHandle));
		}
	}

	DynamicModel::DynamicModel(const Ref<MeshFile> &meshSource, const std::vector<uint32> &submeshes)
		: m_MeshSource(meshSource)
	{
		// Generate a new asset handle
		Handle = {};

		SetSubmeshes(submeshes);

		const auto &meshMaterials = meshSource->GetMaterials();
		m_Materials = Ref<MaterialTable>::Create((uint32)meshMaterials.size());
		for (size_t i = 0; i < meshMaterials.size(); i++)
		{
			AssetHandle materialHandle = AssetManager::Get()->CreateMemoryOnlyAsset<MaterialAsset>(meshMaterials[i]);
			m_Materials->SetMaterial((uint32)i, AssetManager::Get()->GetAsset<MaterialAsset>(materialHandle));
		}
	}

	DynamicModel::DynamicModel(const Ref<DynamicModel> &other)
		: m_MeshSource(other->m_MeshSource), m_Materials(other->m_Materials)
	{
		SetSubmeshes(other->m_Submeshes);
	}

	DynamicModel::~DynamicModel()
	{
		Release();
	}

	void DynamicModel::Release()
	{
		m_Submeshes.clear();
		m_Submeshes.shrink_to_fit();

		m_Materials->Clear();
	}

	void DynamicModel::Invalidate()
	{
		Release();

		SetSubmeshes({});

		const auto &meshMaterials = m_MeshSource->GetMaterials();
		m_Materials = Ref<MaterialTable>::Create((uint32)meshMaterials.size());
		for (size_t i = 0; i < meshMaterials.size(); i++)
		{
			AssetHandle materialHandle = AssetManager::Get()->CreateMemoryOnlyAsset<MaterialAsset>(meshMaterials[i]);
			m_Materials->SetMaterial((uint32)i, AssetManager::Get()->GetAsset<MaterialAsset>(materialHandle));
		}
	}

	void DynamicModel::SetSubmeshes(const std::vector<uint32> &submeshes)
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
	void DynamicModel::SetMeshAsset(const Ref<MeshFile> &meshSource)
	{
		m_MeshSource = meshSource;
		Invalidate();
	}
}

