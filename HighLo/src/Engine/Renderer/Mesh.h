#pragma once
#include "Engine/Renderer/VertexData.h"
#include "Engine/Renderer/VertexArray.h"

namespace highlo
{
	struct MeshData
	{
		std::vector<Vertex> m_Vertices;
		std::vector<int>	m_Indices;
	};

	class Mesh
	{
	public:
		static Ref<Mesh> Create(const MeshData& data);

		MeshData& GetMeshData() { return m_MeshData; }
		void UpdateMeshData(std::vector<Vertex>& data);
		void UpdateMeshData();

		const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }

	private:
		Mesh(const MeshData& data)
			: m_MeshData(data) {}

		MeshData m_MeshData;

	private:
		Ref<VertexArray> m_VertexArray;
	};
}
