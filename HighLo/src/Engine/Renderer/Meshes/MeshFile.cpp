// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "MeshFile.h"

#include "Engine/Renderer/Renderer.h"

namespace highlo
{
	MeshFile::MeshFile(const FileSystemPath &filePath, bool shouldBeAnimated)
	{
		m_MeshShader = shouldBeAnimated ? Renderer::GetShaderLibrary()->Get("HighLoPBRAnimated") : Renderer::GetShaderLibrary()->Get("HighLoPBR");
		m_MeshLoader = MeshLoader::Create(filePath, m_MeshShader);
		m_IsAnimated = m_MeshLoader->IsAnimated();
		m_SubMeshes = m_MeshLoader->GetSubmeshes();
		m_InverseTransform.SetTransform(m_MeshLoader->GetInverseTransform());

		// actual rendering-api specific draw data
		m_VertexBuffer = m_MeshLoader->GetVertexBuffer();
		m_IndexBuffer = m_MeshLoader->GetIndexBuffer();
		m_VertexBufferLayout = m_MeshLoader->GetLayout();

		// Engine-specific draw data
		m_StaticVertices = m_MeshLoader->GetStaticVertices();
		m_AnimatedVertices = m_MeshLoader->GetAnimatedVertices();
		m_Indices = m_MeshLoader->GetIndices();
		m_BoneMapping = m_MeshLoader->GetBoneMappings();
		m_BoneInfos = m_MeshLoader->GetBoneInfos();
		m_BoneCount = m_MeshLoader->GetBoneCount();
		m_BoundingBox = m_MeshLoader->GetBoundingBox();

		m_AnimationDuration = m_MeshLoader->GetAnimationDuration();
		m_TicksPerSecond = m_MeshLoader->GetTicksPerSecond();
		m_BoneTransforms = m_MeshLoader->GetBoneTransforms();

		// Materials
		m_Textures = m_MeshLoader->GetTextures();
		m_Materials = m_MeshLoader->GetMaterials();
		m_TriangleCache = m_MeshLoader->GetTriangleCache();
		m_NormalMaps = m_MeshLoader->GetNormalMaps();
	}

	MeshFile::MeshFile(const std::vector<Vertex> &vertices, const std::vector<VertexIndex> &indices)
		: m_StaticVertices(vertices), m_Indices(indices)
	{
		Mesh submesh;
		submesh.BaseVertex = 0;
		submesh.BaseIndex = 0;
		submesh.MaterialIndex = 0;
		submesh.IndexCount = (uint32)indices.size() * 3u;
		submesh.VertexCount = (uint32)vertices.size();
		submesh.WorldTransform = Transform::FromPosition({ 0.0f, 0.0f, 0.0f });
		m_SubMeshes.push_back(submesh);

		m_VertexBuffer = VertexBuffer::Create(m_StaticVertices.data(), (uint32) (m_StaticVertices.size() * sizeof(Vertex)));
		m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), (uint32) (m_Indices.size() * sizeof(VertexIndex)));
		m_VertexBufferLayout = BufferLayout::GetStaticShaderLayout();
	}

	MeshFile::MeshFile(const std::vector<Vertex> &vertices, const std::vector<VertexIndex> &indices, const AABB &aabb)
		: m_StaticVertices(vertices), m_Indices(indices)
	{
		Mesh submesh;
		submesh.BaseVertex = 0;
		submesh.BaseIndex = 0;
		submesh.MaterialIndex = 0;
		submesh.IndexCount = (uint32) indices.size() * 3u;
		submesh.VertexCount = (uint32)vertices.size();
		submesh.WorldTransform = Transform::FromPosition({ 0.0f, 0.0f, 0.0f });
		m_SubMeshes.push_back(submesh);

		m_VertexBuffer = VertexBuffer::Create(m_StaticVertices.data(), (uint32) (m_StaticVertices.size() * sizeof(Vertex)));
		m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), (uint32) (m_Indices.size() * sizeof(VertexIndex)));
		m_VertexBufferLayout = BufferLayout::GetStaticShaderLayout();
		m_BoundingBox = aabb;
	}

	MeshFile::MeshFile(const std::vector<Vertex> &vertices, const std::vector<VertexIndex> &indices, const Transform &transform)
		: m_StaticVertices(vertices), m_Indices(indices), m_IsAnimated(false)
	{
		Mesh submesh;
		submesh.BaseVertex = 0;
		submesh.BaseIndex = 0;
		submesh.MaterialIndex = 0;
		submesh.IndexCount = (uint32)indices.size() * 3u;
		submesh.VertexCount = (uint32)vertices.size();
		submesh.WorldTransform = transform;
		m_SubMeshes.push_back(submesh);

		m_VertexBuffer = VertexBuffer::Create(m_StaticVertices.data(), (uint32)(m_StaticVertices.size() * sizeof(Vertex)));
		m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), (uint32)(m_Indices.size() * sizeof(VertexIndex)));
		m_VertexBufferLayout = BufferLayout::GetStaticShaderLayout();

		// TODO: generate bounding box for submeshes,
	}

	MeshFile::MeshFile(const std::vector<Vertex> &vertices, const std::vector<VertexIndex> &indices, const std::vector<Mesh> &subMeshes)
		: m_StaticVertices(vertices), m_Indices(indices), m_SubMeshes(subMeshes), m_IsAnimated(false)
	{
		m_VertexBuffer = VertexBuffer::Create(m_StaticVertices.data(), (uint32)(m_StaticVertices.size() * sizeof(Vertex)));
		m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), (uint32)(m_Indices.size() * sizeof(VertexIndex)));
		m_VertexBufferLayout = BufferLayout::GetStaticShaderLayout();

		// TODO: generate bounding box for submeshes,
	}

	MeshFile::~MeshFile()
	{
	}

	void MeshFile::DumpVertexBuffer()
	{
		HL_CORE_TRACE("Mesh: {0}", **m_FilePath);
		if (m_IsAnimated)
		{
			for (uint32 i = 0; i < m_AnimatedVertices.size(); ++i)
			{
				auto &vertex = m_AnimatedVertices[i];
				HL_CORE_TRACE("Vertex: {0}", i);
				HL_CORE_TRACE("Position: {0}, {1}, {2}", vertex.Position.x, vertex.Position.y, vertex.Position.z);
				HL_CORE_TRACE("Normal: {0}, {1}, {2}", vertex.Normal.x, vertex.Normal.y, vertex.Normal.z);
				HL_CORE_TRACE("Binormal: {0}, {1}, {2}", vertex.BiNormal.x, vertex.BiNormal.y, vertex.BiNormal.z);
				HL_CORE_TRACE("Tangent: {0}, {1}, {2}", vertex.Tangent.x, vertex.Tangent.y, vertex.Tangent.z);
				HL_CORE_TRACE("TexCoord: {0}, {1}", vertex.TexCoord.x, vertex.TexCoord.y);
			}
		}
		else
		{
			for (uint32 i = 0; i < m_StaticVertices.size(); ++i)
			{
				auto &vertex = m_StaticVertices[i];
				HL_CORE_TRACE("Vertex: {0}", i);
				HL_CORE_TRACE("Position: {0}, {1}, {2}", vertex.Position.x, vertex.Position.y, vertex.Position.z);
				HL_CORE_TRACE("Normal: {0}, {1}, {2}", vertex.Normal.x, vertex.Normal.y, vertex.Normal.z);
				HL_CORE_TRACE("Binormal: {0}, {1}, {2}", vertex.BiNormal.x, vertex.BiNormal.y, vertex.BiNormal.z);
				HL_CORE_TRACE("Tangent: {0}, {1}, {2}", vertex.Tangent.x, vertex.Tangent.y, vertex.Tangent.z);
				HL_CORE_TRACE("TexCoord: {0}, {1}", vertex.TexCoord.x, vertex.TexCoord.y);
			}
		}
	}
	
	void MeshFile::ManipulateBoneTransform(float time)
	{
		m_MeshLoader->ManipulateBoneTransform(time, m_BoneTransforms);
	}

	Ref<MeshFile> MeshFile::Create(const FileSystemPath &filePath, bool shouldBeAnimated)
	{
		return Ref<MeshFile>::Create(filePath, shouldBeAnimated);
	}

	Ref<MeshFile> MeshFile::Create(const std::vector<Vertex> &vertices, const std::vector<VertexIndex> &indices)
	{
		return Ref<MeshFile>::Create(vertices, indices);
	}

	Ref<MeshFile> MeshFile::Create(const std::vector<Vertex> &vertices, const std::vector<VertexIndex> &indices, const AABB &aabb)
	{
		return Ref<MeshFile>::Create(vertices, indices, aabb);
	}

	Ref<MeshFile> MeshFile::Create(const std::vector<Vertex> &vertices, const std::vector<VertexIndex> &indices, const Transform &transform)
	{
		return Ref<MeshFile>::Create(vertices, indices, transform);
	}

	Ref<MeshFile> MeshFile::Create(const std::vector<Vertex> &vertices, const std::vector<VertexIndex> &indices, const std::vector<Mesh> &subMeshes)
	{
		return Ref<MeshFile>::Create(vertices, indices, subMeshes);
	}
}

