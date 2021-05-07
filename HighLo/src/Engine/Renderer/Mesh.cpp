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
			{ "BONE_IDS", ShaderDataType::Int4	 },
			{ "BONE_WEIGHTS", ShaderDataType::Float4 }
		};

		Ref<VertexBuffer> vertex_buffer = VertexBuffer::Create(const_cast<MeshData&>(data).m_Vertices);
		vertex_buffer->SetLayout(layout);

		Ref<IndexBuffer> index_buffer = IndexBuffer::Create(const_cast<MeshData&>(data).m_Indices);

		instance->m_VertexArray->AddVertexBuffer(vertex_buffer);
		instance->m_VertexArray->SetIndexBuffer(index_buffer);

		instance->m_VertexArray->Unbind();

		instance->material = Material::Create();

		return instance;
	}

	void Mesh::UpdateMeshData(std::vector<Vertex>& data)
	{
		m_VertexArray->GetVertexBuffers()[0]->UpdateContents(data);
	}
	
	void Mesh::UpdateMeshData()
	{
		m_VertexArray->GetVertexBuffers()[0]->UpdateContents(m_MeshData.m_Vertices);
	}
}
