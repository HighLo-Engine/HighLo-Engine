#pragma once

#include "Mesh.h"

#include "Engine/Assets/Asset.h"
#include "Engine/Renderer/MaterialTable.h"
#include "Engine/Renderer/Meshes/MeshFile.h"

namespace highlo
{
	class StaticModel : public Asset
	{
	public:

		// Constructors
		explicit StaticModel(Ref<MeshFile> &mesh);
		StaticModel(Ref<MeshFile> &mesh, const std::vector<uint32> &subMeshIndices);
		StaticModel(const Ref<StaticModel> &other);
		virtual ~StaticModel();

		// Submesh indices
		std::vector<uint32> &GetSubmeshIndices() { return m_SubMeshIndices; }
		const std::vector<uint32> &GetSubmeshIndices() const { return m_SubMeshIndices; }
		
		void SetSubmeshIndices(const std::vector<uint32> &submeshIndices);

		// MeshFile
		Ref<MeshFile> &Get() { return m_MeshFile; }
		const Ref<MeshFile> &Get() const { return m_MeshFile; }

		void Set(const Ref<MeshFile> &subMesh) { m_MeshFile = subMesh; }

		// Materials
		Ref<MaterialTable> GetMaterials() const { return m_Materials; }

		// Inherited via Asset
		static AssetType GetStaticType() { return AssetType::StaticMesh; }
		virtual AssetType GetAssetType() const override { return GetStaticType(); }

	private:

		Ref<MeshFile> m_MeshFile;
		std::vector<uint32> m_SubMeshIndices;

		Ref<MaterialTable> m_Materials;

		friend class Scene;
		friend class SceneRenderer;
	};
}

