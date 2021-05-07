#pragma once
#include "Engine/Renderer/VertexData.h"
#include "Engine/Renderer/VertexArray.h"
#include "Material.h"
#include "Animation.h"

namespace highlo
{
	struct MeshData
	{
		std::vector<Vertex> m_Vertices;
		std::vector<int32>	m_Indices;
	};

	class Mesh : public IsSharedReference
	{
	public:
		static Ref<Mesh> Create(const MeshData& data);

		MeshData& GetMeshData() { return m_MeshData; }
		void UpdateMeshData(std::vector<Vertex>& data);
		void UpdateMeshData();

		Ref<VertexArray>& GetVertexArray() { return m_VertexArray; }

		Ref<Animation> m_Animation = nullptr;
		Ref<Material> m_Material;

	private:
		Mesh(const MeshData& data)
			: m_MeshData(data) {}

		MeshData m_MeshData;

	private:
		Ref<VertexArray> m_VertexArray;
	};
}
