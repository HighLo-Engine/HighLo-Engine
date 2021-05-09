#include "HighLoPch.h"
#include "Mesh.h"

namespace highlo
{
	Ref<Mesh> Mesh::Create(const MeshData& data)
	{
		Ref<Mesh> instance = Ref<Mesh>(new Mesh(data));
		instance->m_VertexArray = VertexArray::Create();

		BufferLayout layout = {
			{ "POSITION", ShaderDataType::Float3 },
			{ "UV"		, ShaderDataType::Float2 },
			{ "NORMAL"	, ShaderDataType::Float3 },
			{ "TANGENT"	, ShaderDataType::Float3 },
			{ "BINORMAL", ShaderDataType::Float3 },
			{ "BONE_IDS", ShaderDataType::Int4	},
			{ "BONE_WEIGHTS", ShaderDataType::Float4 }
		};

		Ref<VertexBuffer> vertex_buffer = VertexBuffer::Create(const_cast<MeshData&>(data).m_Vertices);
		vertex_buffer->SetLayout(layout);

		Ref<IndexBuffer> index_buffer = IndexBuffer::Create(const_cast<MeshData&>(data).m_Indices);

		instance->m_VertexArray->AddVertexBuffer(vertex_buffer);
		instance->m_VertexArray->SetIndexBuffer(index_buffer);

		instance->m_VertexArray->Unbind();

		instance->m_Material = Material::Create();

		return instance;
	}

	void Mesh::Update(Timestep timestep)
	{
		if (m_Animation != nullptr)
		{
			m_Animation->Update(timestep);
		}
	}

	void Mesh::UpdateMeshData(std::vector<Vertex>& data)
	{
		m_VertexArray->GetVertexBuffers()[0]->UpdateContents(data);
	}
	
	void Mesh::UpdateMeshData()
	{
		m_VertexArray->GetVertexBuffers()[0]->UpdateContents(m_MeshData.m_Vertices);
	}

	void Mesh::SetTexture(int32 type, Ref<Texture> texture)
	{
		m_Material->SetTexture(type, texture);
	}

	MaterialRenderProperties &Mesh::GetMaterialProperties()
	{
		return m_Material->Properties.m_RenderProperties;
	}

	MaterialPhysicalProperties &Mesh::GetPhysicsProperties()
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

	Ref<Mesh> Mesh::Copy(const Ref<Mesh>& src)
	{
		return src->Copy();
	}

	void Mesh::Copy(const Ref<Mesh>& src, Ref<Mesh>& dest)
	{
		dest = src->Copy();
	}
}
