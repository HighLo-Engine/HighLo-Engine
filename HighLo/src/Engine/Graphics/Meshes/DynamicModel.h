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
	class DynamicModel : public Asset
	{
	public:

		// Constructors
		explicit DynamicModel(Ref<MeshFile> &mesh);
		DynamicModel(Ref<MeshFile> &mesh, const std::vector<uint32> &submeshIndices);
		DynamicModel(const Ref<DynamicModel> &other);
		virtual ~DynamicModel();

		// update function
		void OnUpdate(Timestep ts);

		// Submesh indices
		std::vector<uint32> &GetSubmeshIndices() { return m_SubMeshIndices; }
		const std::vector<uint32> &GetSubmeshIndices() const { return m_SubMeshIndices; }

		void SetSubmeshIndices(const std::vector<uint32> &indices);
		bool IsAnimated() const { return m_MeshFile->m_IsAnimated; }

		std::vector<glm::mat4> &GetBoneTransforms() { return m_MeshFile->m_BoneTransforms; }
		const std::vector<glm::mat4> &GetBoneTransforms() const { return m_MeshFile->m_BoneTransforms; }

		// MeshFile
		Ref<MeshFile> &Get() { return m_MeshFile; }
		const Ref<MeshFile> &Get() const { return m_MeshFile; }

		void Set(const Ref<MeshFile> &subMesh) { m_MeshFile = subMesh; }

		// Materials
		Ref<MaterialTable> GetMaterials() const { return m_Materials; }

		// Inherited via Asset
		static AssetType GetStaticType() { return AssetType::DynamicMesh; }
		virtual AssetType GetAssetType() const override { return GetStaticType(); }

	private:

		Ref<MeshFile> m_MeshFile;
		std::vector<uint32> m_SubMeshIndices;

		uint32 m_BoneCount = 0;
		std::vector<BoneInfo> m_BoneInfos;

		Ref<MaterialTable> m_Materials;

		bool m_IsAnimated = false;			// Whether or not this model is animated
		bool m_AnimationPlaying = false;	// Whether or not the animation is currently playing
		float m_AnimationTime = 0.0f;		// The duration of the current animation
		float m_WorldTime = 0.0f;			// The global deltatime
		float m_TimeMultiplier = 1.0f;		// used to control the speed of the animations

		friend class Scene;
		friend class SceneRenderer;
	};
}

