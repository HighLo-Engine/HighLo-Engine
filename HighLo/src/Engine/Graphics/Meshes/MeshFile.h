// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-12-21) initial release
//

#pragma once

#include "Mesh.h"

#include "Engine/Assets/Asset.h"
#include "Engine/Core/FileSystemPath.h"
#include "Engine/Graphics/Shaders/Shader.h"
#include "Engine/Graphics/Material.h"
#include "Engine/Graphics/VertexBuffer.h"
#include "Engine/Graphics/IndexBuffer.h"

#include "Engine/Animation/Animation.h"
#include "Engine/Animation/Skeleton.h"

namespace highlo
{
	/// <summary>
	/// Represents an actual asset file on the disk
	/// </summary>
	class MeshFile : public Asset
	{
	public:

		MeshFile() = default;
		MeshFile(const std::vector<Vertex> &vertices, const std::vector<Index> &indices, const glm::mat4 &transform);
		MeshFile(const std::vector<Vertex> &vertices, const std::vector<Index> &indices, const std::vector<Mesh> &submeshes);
		virtual ~MeshFile();

		void DumpVertexBuffer();

		std::vector<Mesh> &GetSubmeshes() { return m_Submeshes; }
		const std::vector<Mesh> &GetSubmeshes() const { return m_Submeshes; }

		const std::vector<Vertex> &GetVertices() const { return m_Vertices; }
		const std::vector<Index> &GetIndices() const { return m_Indices; }

		bool HasSkeleton() const { return (bool)m_Skeleton; }
		bool IsSubmeshRigged(uint32 submeshIndex) const { return m_Submeshes[submeshIndex].IsRigged; }
		const Skeleton &GetSkeleton() const { HL_ASSERT(m_Skeleton, "Attempted to access null skeleton!"); return *m_Skeleton; }
		bool IsCompatibleSkeleton(const uint32 animationIndex, const Skeleton &skeleton) const;
		uint32 GetAnimationCount() const;
		const Animation &GetAnimation(const uint32 animationIndex, const Skeleton &skeleton) const;
		const std::vector<BoneInfluence> &GetBoneInfluences() const { return m_BoneInfluences; }

		std::vector<Ref<Material>> &GetMaterials() { return m_Materials; }
		const std::vector<Ref<Material>> &GetMaterials() const { return m_Materials; }
		const FileSystemPath &GetFilePath() const { return m_FilePath; }

		const std::vector<Triangle> &GetTriangleCache(uint32 index) const { return m_TriangleCache.at(index); }

		Ref<VertexBuffer> &GetVertexBuffer() { return m_VertexBuffer; }
		Ref<VertexBuffer> &GetBoneInfluenceBuffer() { return m_BoneInfluenceBuffer; }
		Ref<IndexBuffer> &GetIndexBuffer() { return m_IndexBuffer; }

		const AABB &GetBoundingBox() const { return m_BoundingBox; }
		const MeshNode &GetRootNode() const { return m_Nodes[0]; }
		const std::vector<MeshNode> &GetNodes() const { return m_Nodes; }

		// Inherited from Asset
		static AssetType GetStaticType() { return AssetType::MeshAsset; }
		virtual AssetType GetAssetType() const override { return GetStaticType(); }

	private:

		std::vector<Mesh> m_Submeshes;

		Ref<VertexBuffer> m_VertexBuffer;
		Ref<VertexBuffer> m_BoneInfluenceBuffer;
		Ref<IndexBuffer> m_IndexBuffer;

		std::vector<Vertex> m_Vertices;
		std::vector<Index> m_Indices;

		std::vector<BoneInfluence> m_BoneInfluences;
		std::vector<BoneInfo> m_BoneInfo;
		mutable UniqueReference<Skeleton> m_Skeleton;
		mutable std::vector<UniqueReference<Animation>> m_Animations;

		std::vector<Ref<Material>> m_Materials;
		std::unordered_map<uint32, std::vector<Triangle>> m_TriangleCache;

		AABB m_BoundingBox;
		FileSystemPath m_FilePath;
		std::vector<MeshNode> m_Nodes;

		// TODO: TEMP
		bool m_Runtime = false;

		friend class Scene;
		friend class SceneRenderer;
		friend class Renderer;
		friend class VulkanRenderer;
		friend class OpenGLRenderer;
		friend class SceneHierarchyPanel;
		friend class MeshViewerPanel;
		friend class AssimpMeshImporter;
	};
}

