// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-12-21) initial release
//

#pragma once

#include "Mesh.h"

#include "Engine/Assets/Asset.h"
#include "Engine/Graphics/MaterialTable.h"
#include "Engine/Graphics/Meshes/MeshFile.h"

namespace highlo
{
	// Static Mesh - no skeletal animation, flattened hierarchy
	class StaticModel : public Asset
	{
	public:

		explicit StaticModel(const Ref<MeshFile> &meshSource);
		StaticModel(const Ref<MeshFile> &meshSource, const std::vector<uint32> &submeshes);
		StaticModel(const Ref<StaticModel> &other);
		virtual ~StaticModel();

		void Release();
		void Invalidate();

		std::vector<uint32> &GetSubmeshes() { return m_Submeshes; }
		const std::vector<uint32> &GetSubmeshes() const { return m_Submeshes; }

		// Pass in an empty vector to set ALL submeshes for MeshSource
		void SetSubmeshes(const std::vector<uint32> &submeshes);

		Ref<MeshFile> &Get() { return m_MeshSource; }
		const Ref<MeshFile> &Get() const { return m_MeshSource; }
		void SetMeshAsset(const Ref<MeshFile> &meshAsset);

		Ref<MaterialTable> GetMaterials() const { return m_Materials; }

		static AssetType GetStaticType() { return AssetType::StaticMesh; }
		virtual AssetType GetAssetType() const override { return GetStaticType(); }

	private:

		Ref<MeshFile> m_MeshSource;
		std::vector<uint32> m_Submeshes;

		// Materials
		Ref<MaterialTable> m_Materials;

		friend class Scene;
		friend class Renderer;
		friend class VulkanRenderer;
		friend class OpenGLRenderer;
		friend class SceneHierarchyPanel;
		friend class MeshViewerPanel;
	};
}

