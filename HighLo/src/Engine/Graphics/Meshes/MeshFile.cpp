// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MeshFile.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Scene/Project.h"

#include "Engine/ThirdParty/Assimp/AssimpMeshLoader.h"
#include "Engine/Core/UniqueReference.h"
#include "Engine/Assets/AssetManager.h"

#define MESH_FILE_LOG_PREFIX "         >"

#define MESH_DEBUG_LOG 0
#if MESH_DEBUG_LOG
#define HL_MESH_LOG(...) HL_CORE_TRACE(__VA_ARGS__)
#define HL_MESH_ERROR(...) HL_CORE_ERROR(__VA_ARGS__)
#else
#define HL_MESH_LOG(...)
#define HL_MESH_ERROR(...)
#endif

namespace highlo
{
	MeshFile::MeshFile(const std::vector<Vertex> &vertices, const std::vector<Index> &indices, const glm::mat4 &transform)
		: m_Vertices(vertices), m_Indices(indices)
	{
		// Generate a new asset handle
		Handle = {};

		Mesh submesh;
		submesh.BaseVertex = 0;
		submesh.BaseIndex = 0;
		submesh.IndexCount = (uint32)(indices.size() * 3);
		submesh.Transform = transform;
		m_Submeshes.push_back(submesh);

		m_VertexBuffer = VertexBuffer::Create(m_Vertices.data(), (uint32)(m_Vertices.size() * sizeof(Vertex)));
		m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), (uint32)(m_Indices.size() * sizeof(Index)));

		// TODO: generate bounding box for submeshes, etc.
	}

	MeshFile::MeshFile(const std::vector<Vertex> &vertices, const std::vector<Index> &indices, const std::vector<Mesh> &submeshes)
		: m_Vertices(vertices), m_Indices(indices), m_Submeshes(submeshes)
	{
		// Generate a new asset handle
		Handle = {};

		m_VertexBuffer = VertexBuffer::Create(m_Vertices.data(), (uint32)(m_Vertices.size() * sizeof(Vertex)));
		m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), (uint32)(m_Indices.size() * sizeof(Index)));

		// TODO: generate bounding box for submeshes, etc.
	}

	MeshFile::~MeshFile()
	{
	}

	static HLString LevelToSpaces(uint32 level)
	{
		std::string result = "";
		for (uint32 i = 0; i < level; i++)
			result += "--";
		return result;
	}

	void MeshFile::DumpVertexBuffer()
	{
		HL_MESH_LOG("------------------------------------------------------");
		HL_MESH_LOG("Vertex Buffer Dump");
		HL_MESH_LOG("Mesh: {0}", **m_FilePath);
		for (size_t i = 0; i < m_Vertices.size(); i++)
		{
			auto &vertex = m_Vertices[i];
			HL_MESH_LOG("Vertex: {0}", i);
			HL_MESH_LOG("Position: {0}, {1}, {2}", vertex.Position.x, vertex.Position.y, vertex.Position.z);
			HL_MESH_LOG("Normal: {0}, {1}, {2}", vertex.Normal.x, vertex.Normal.y, vertex.Normal.z);
			HL_MESH_LOG("Binormal: {0}, {1}, {2}", vertex.Binormal.x, vertex.Binormal.y, vertex.Binormal.z);
			HL_MESH_LOG("Tangent: {0}, {1}, {2}", vertex.Tangent.x, vertex.Tangent.y, vertex.Tangent.z);
			HL_MESH_LOG("TexCoord: {0}, {1}", vertex.Texcoord.x, vertex.Texcoord.y);
			HL_MESH_LOG("--");
		}
		HL_MESH_LOG("------------------------------------------------------");
	}


	// TODO: this is temporary.. and will eventually be replaced with some kind of skeleton retargeting
	bool MeshFile::IsCompatibleSkeleton(const uint32 animationIndex, const Skeleton &skeleton) const
	{
		if (!m_Skeleton)
		{
			HL_ASSERT(!m_Runtime);
		
		//	auto path = Project::GetEditorAssetManager()->GetFileSystemPath(Handle);

			Ref<const MeshFile> instance = this;
			AssetMetaData meta = AssetManager::Get()->GetMetaData(instance);
			FileSystemPath path = AssetManager::Get()->GetFileSystemPath(meta);
			
			AssimpMeshImporter importer(path);
			return importer.IsCompatibleSkeleton(animationIndex, skeleton);
		}

		return m_Skeleton->GetBoneNames() == skeleton.GetBoneNames();
	}

	uint32 MeshFile::GetAnimationCount() const
	{
		if (m_Runtime)
			return (uint32)m_Animations.size();

	//	auto path = Project::GetEditorAssetManager()->GetFileSystemPath(Handle);
		
		Ref<const MeshFile> instance = this;
		AssetMetaData meta = AssetManager::Get()->GetMetaData(instance);
		FileSystemPath path = AssetManager::Get()->GetFileSystemPath(meta);

		AssimpMeshImporter importer(path);
		return importer.GetAnimationCount();
	}

	const Animation &MeshFile::GetAnimation(const uint32 animationIndex, const Skeleton &skeleton) const
	{
		if (!m_Skeleton)
		{
			HL_ASSERT(!m_Runtime);

		//	auto path = Project::GetEditorAssetManager()->GetFileSystemPath(Handle);

			Ref<const MeshFile> instance = this;
			AssetMetaData meta = AssetManager::Get()->GetMetaData(instance);
			FileSystemPath path = AssetManager::Get()->GetFileSystemPath(meta);
			
			AssimpMeshImporter importer(path);
			m_Skeleton = UniqueReference<Skeleton>::Create(skeleton);
			importer.ImportAnimations(animationIndex, *m_Skeleton, m_Animations);
		}

		HL_ASSERT(animationIndex < m_Animations.size(), "Animation index out of range!");
		HL_ASSERT(m_Animations[animationIndex], "Attempted to access null animation!");
		return *m_Animations[animationIndex];
	}

}

