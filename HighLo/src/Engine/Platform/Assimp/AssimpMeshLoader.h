#pragma once

#include <glm/glm.hpp>

#include "Engine/Loaders/MeshLoader.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

struct aiNode;
struct aiAnimation;
struct aiNodeAnim;
struct aiScene;

namespace highlo
{
	/// <summary>
	/// Represents a mesh that is specific to Assimp's format
	/// </summary>
	class AssimpMeshLoader : public MeshLoader
	{
	public:

		AssimpMeshLoader(const FileSystemPath &filePath, const Ref<Shader> &shader);
		virtual ~AssimpMeshLoader();
		
		// Draw data
		const Ref<VertexBuffer> &GetVertexBuffer() const override { return m_VertexBuffer; }
		const Ref<IndexBuffer> &GetIndexBuffer() const override { return m_IndexBuffer; }
		virtual const BufferLayout &GetLayout() const override { return m_Layout; }

		// Materials
		const std::vector<Ref<Material>> &GetMaterials() const override { return m_Materials; }
		const std::vector<Ref<Texture2D>> &GetTextures() const override { return m_Textures; }
		virtual const std::vector<Ref<Texture2D>> &GetNormalMaps() const override { return m_NormalMaps; }

		// Anim
		const glm::mat4 &GetInverseTransform() const override { return m_InverseTransform; }
		const std::vector<glm::mat4> &GetBoneTransforms() const override { return m_BoneTransforms; }
		const std::unordered_map<uint32, std::vector<Triangle>> &GetTriangleCache() const override { return m_TriangleCache; }
		const std::vector<Mesh> &GetSubmeshes() const override { return m_Submeshes; }

		uint32 GetBoneCount() const override { return m_BoneCount; }
		const std::unordered_map<HLString, uint32> &GetBoneMappings() const override { return m_BoneMapping; }
		const std::vector<BoneInfo> &GetBoneInfos() const override { return m_BoneInfos; }
		bool IsAnimated() const override { return m_IsAnimated; }
		const FileSystemPath &GetFilePath() const override { return m_FilePath; }
		const AABB &GetBoundingBox() const override { return m_BoundingBox; }

		virtual const std::vector<Vertex> &GetStaticVertices() const override { return m_StaticVertices; }
		virtual const std::vector<AnimatedVertex> &GetAnimatedVertices() const override { return m_AnimatedVertices; }
		virtual const std::vector<VertexIndex> &GetIndices() const override { return m_Indices; }

		virtual float GetTicksPerSecond() const override { return m_TicksPerSecond; }
		virtual float GetAnimationDuration() const override { return m_AnimationDuration; }

		virtual void ManipulateBoneTransform(float time, std::vector<glm::mat4> &outTransforms) override;

	private:

		void BoneTransform(float time);
		void ReadNodeHierarchy(float animTime, const aiNode *node, const glm::mat4 &parentTransform);
		void TraverseNodes(aiNode *node, const glm::mat4 &parentTransform = glm::mat4(1.0f), uint32 level = 0);

		const aiNodeAnim *FindNodeAnim(const aiAnimation *anim, const HLString &name);
		uint32 FindPosition(float animTime, const aiNodeAnim *anim);
		uint32 FindScale(float animTime, const aiNodeAnim *anim);
		uint32 FindRotation(float animTime, const aiNodeAnim *anim);

		glm::vec3 InterpolateTranslation(float animTime, const aiNodeAnim *anim);
		glm::vec3 InterpolateScale(float animTime, const aiNodeAnim *anim);
		glm::quat InterpolateRotation(float animTime, const aiNodeAnim *anim);

	private:

		UniqueRef<Assimp::Importer> m_Importer;
		const aiScene *m_Scene;
		std::unordered_map<aiNode*, std::vector<uint32>> m_NodeMap;

		FileSystemPath m_FilePath;
		std::vector<glm::mat4> m_BoneTransforms;
		std::vector<Vertex> m_StaticVertices;
		std::vector<AnimatedVertex> m_AnimatedVertices;
		std::vector<VertexIndex> m_Indices;
		std::vector<Mesh> m_Submeshes;
		std::unordered_map<uint32, std::vector<Triangle>> m_TriangleCache;
		glm::mat4 m_InverseTransform;

		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		BufferLayout m_Layout;

		// Materials
		std::vector<Ref<Material>> m_Materials;
		std::vector<Ref<Texture2D>> m_Textures;
		std::vector<Ref<Texture2D>> m_NormalMaps;

		// Anim
		uint32 m_BoneCount = 0;
		std::unordered_map<HLString, uint32> m_BoneMapping;
		std::vector<BoneInfo> m_BoneInfos;
		bool m_IsAnimated = false;
		float m_AnimationDuration = 0.0f;
		float m_TicksPerSecond = 0.0f;
		AABB m_BoundingBox;

		uint32 m_MeshImportFlags = 
			aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
			aiProcess_Triangulate |             // Make sure we're triangles
			aiProcess_SortByPType |             // Split meshes by primitive type
			aiProcess_GenNormals |              // Make sure we have legit normals
			aiProcess_GenUVCoords |             // Convert UVs if required 
			aiProcess_OptimizeMeshes |          // Batch draws where possible
			aiProcess_JoinIdenticalVertices |
			aiProcess_ValidateDataStructure;    // Validation
	};
}

