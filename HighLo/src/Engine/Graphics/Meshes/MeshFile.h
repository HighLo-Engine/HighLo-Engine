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

#include "Engine/Loaders/MeshLoader.h"

namespace highlo
{
	/// <summary>
	/// Represents an actual asset file on the disk
	/// </summary>
	class MeshFile : public Asset
	{
	public:
	
		MeshFile(const FileSystemPath &filePath, bool shouldBeAnimated = false);
		MeshFile(const std::vector<Vertex> &vertices, const std::vector<VertexIndex> &indices);
		MeshFile(const std::vector<Vertex> &vertices, const std::vector<VertexIndex> &indices, const AABB &aabb);
		MeshFile(const std::vector<Vertex> &vertices, const std::vector<VertexIndex> &indices, const Transform &transform);
		MeshFile(const std::vector<Vertex> &vertices, const std::vector<VertexIndex> &indices, const std::vector<Mesh> &subMeshes);
		virtual ~MeshFile();

		// Debug only
		void DumpVertexBuffer();

		std::vector<Mesh> &GetSubmeshes() { return m_SubMeshes; }
		const std::vector<Mesh> &GetSubmeshes() const { return m_SubMeshes; }

		Ref<Shader> GetMeshShader() { return m_MeshShader; }
		std::vector<Ref<Material>> &GetMaterials() { return m_Materials; }
		const std::vector<Ref<Material>> &GetMaterials() const { return m_Materials; }
		const std::vector<Ref<Texture2D>> &GetTextures() const { return m_Textures; }
		const FileSystemPath &GetFilePath() const { return m_FilePath; }

		const std::vector<Triangle> GetTriangleCache(uint32 index) const { HL_ASSERT(index < m_TriangleCache.size()); return m_TriangleCache.at(index); }

		Ref<VertexBuffer> GetVertexBuffer() { return m_VertexBuffer; }
		Ref<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }
		const BufferLayout &GetVertexBufferLayout() const { return m_VertexBufferLayout; }

		const AABB &GetBoundingBox() const { return m_BoundingBox; }

		void ManipulateBoneTransform(float time);

		// Inherited via Asset
		static AssetType GetStaticType() { return AssetType::MeshAsset; }
		virtual AssetType GetAssetType() const override { return GetStaticType(); }

		static Ref<MeshFile> Create(const FileSystemPath &filePath, bool shouldBeAnimated = false);
		static Ref<MeshFile> Create(const std::vector<Vertex> &vertices, const std::vector<VertexIndex> &indices);
		static Ref<MeshFile> Create(const std::vector<Vertex> &vertices, const std::vector<VertexIndex> &indices, const AABB &aabb);
		static Ref<MeshFile> Create(const std::vector<Vertex> &vertices, const std::vector<VertexIndex> &indices, const Transform &transform);
		static Ref<MeshFile> Create(const std::vector<Vertex> &vertices, const std::vector<VertexIndex> &indices, const std::vector<Mesh> &subMeshes);

	private:

		// General infos
		Ref<MeshLoader> m_MeshLoader;
		FileSystemPath m_FilePath;
		std::vector<Mesh> m_SubMeshes;
		Transform m_InverseTransform;

		// actual rendering-api specific draw data
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		BufferLayout m_VertexBufferLayout;

		// Engine-specific draw data
		std::vector<Vertex> m_StaticVertices;
		std::vector<AnimatedVertex> m_AnimatedVertices;
		std::vector<VertexIndex> m_Indices;
		std::unordered_map<HLString, uint32> m_BoneMapping;
		std::vector<glm::mat4> m_BoneTransforms;
		std::vector<BoneInfo> m_BoneInfos;
		uint32 m_BoneCount = 0;
		AABB m_BoundingBox;

		// Materials
		Ref<Shader> m_MeshShader;
		std::vector<Ref<Texture2D>> m_Textures;
		std::vector<Ref<Texture2D>> m_NormalMaps;
		std::vector<Ref<Material>> m_Materials;

		std::unordered_map<uint32, std::vector<Triangle>> m_TriangleCache;

		// Animation
		bool m_IsAnimated = false;
		bool m_AnimIsPlaying = false;
		float m_TimeMuliplier = 1.0f;
		float m_AnimationDuration = 0.0f;
		float m_TicksPerSecond = 0.0f;

		// Relations
		friend class Scene;
		friend class SceneRenderer;
		friend class DynamicModel;
		friend class OpenGLRenderingAPI;
	};
}

