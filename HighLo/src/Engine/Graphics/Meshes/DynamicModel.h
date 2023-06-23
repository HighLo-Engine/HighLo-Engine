// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-11-25) completely refactored to fit the new shading system
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Mesh.h"
#include "MeshFile.h"
#include "Engine/Assets/Asset.h"
#include "Engine/Core/Time.h"
#include "Engine/Graphics/MaterialTable.h"

namespace highlo
{
	// Dynamic Mesh - supports skeletal animation and retains hierarchy
	class DynamicModel : public Asset
	{
	public:

		explicit DynamicModel(const Ref<MeshFile> &meshSource);
		DynamicModel(const Ref<MeshFile> &meshSource, const std::vector<uint32> &submeshes);
		DynamicModel(const Ref<DynamicModel> &other);
		virtual ~DynamicModel();

		void Release();
		void Invalidate();

		bool HasSkeleton() const { return m_MeshSource && m_MeshSource->HasSkeleton(); }

		std::vector<uint32> &GetSubmeshes() { return m_Submeshes; }
		const std::vector<uint32> &GetSubmeshes() const { return m_Submeshes; }

		// Pass in an empty vector to set ALL submeshes for MeshSource
		void SetSubmeshes(const std::vector<uint32> &submeshes);

		Ref<MeshFile> &Get() { return m_MeshSource; }
		const Ref<MeshFile> &Get() const { return m_MeshSource; }
		void SetMeshAsset(const Ref<MeshFile> &meshSource);

		Ref<MaterialTable> GetMaterials() const { return m_Materials; }

		// Inherited from Asset
		static AssetType GetStaticType() { return AssetType::DynamicMesh; }
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

