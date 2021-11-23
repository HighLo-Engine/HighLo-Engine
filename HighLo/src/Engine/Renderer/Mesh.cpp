// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Mesh.h"

namespace highlo
{
	Ref<Mesh> Mesh::Create(const MeshData &data)
	{
		Ref<Mesh> instance = Ref<Mesh>(new Mesh(data));
		instance->m_VertexArray = VertexArray::Create();

		Ref<VertexBuffer> vertex_buffer = VertexBuffer::Create(const_cast<MeshData&>(data).Vertices);
		vertex_buffer->SetLayout(BufferLayout::GetAnimatedShaderLayout());

		Ref<IndexBuffer> index_buffer = IndexBuffer::Create(const_cast<MeshData&>(data).Indices);

		instance->m_VertexArray->AddVertexBuffer(vertex_buffer);
		instance->m_VertexArray->SetIndexBuffer(index_buffer);
		instance->m_VertexArray->Unbind();

		instance->m_Material = Material::Create();

		return instance;
	}

	void Mesh::Update(Timestep timestep)
	{
		if (m_Animation)
			m_Animation->Update(timestep);
	}

	void Mesh::UpdateMeshData(std::vector<Vertex> &data, uint32 meshIndex)
	{
		HL_ASSERT(meshIndex >= 0 && meshIndex < m_VertexArray->GetVertexBuffers().size());
		m_VertexArray->GetVertexBuffers()[meshIndex]->UpdateContents(data);
	}
	
	void Mesh::UpdateMeshData(uint32 meshIndex)
	{
		HL_ASSERT(meshIndex >= 0 && meshIndex < m_VertexArray->GetVertexBuffers().size());
		m_VertexArray->GetVertexBuffers()[meshIndex]->UpdateContents(m_MeshData.Vertices);
	}

	void Mesh::SetTexture(int32 type, Ref<Texture2D> texture)
	{
		m_Material->SetTexture(type, texture);
	}

	MaterialRenderProperties &Mesh::GetMaterialRenderProperties()
	{
		return m_Material->Properties.m_RenderProperties;
	}

	MaterialPhysicalProperties &Mesh::GetMaterialPhysicsProperties()
	{
		return m_Material->Properties.m_PhysicalProperties;
	}

	Ref<Mesh> Mesh::Copy() const
	{
		Ref<Mesh> clone = Ref<Mesh>(new Mesh(m_MeshData));

		clone->m_VertexArray = this->m_VertexArray;
		clone->m_Animation = this->m_Animation;
		clone->m_Material = Material::Create();

		for (uint32 i = 0; i < m_Material->GetTextureCount(); ++i)
			clone->m_Material->SetTexture(i, m_Material->GetTexture(i));

		return clone;
	}

	Ref<Mesh> Mesh::Copy(const Ref<Mesh> &src)
	{
		return src->Copy();
	}

	void Mesh::Copy(const Ref<Mesh> &src, Ref<Mesh> &dest)
	{
		dest = src->Copy();
	}
}
