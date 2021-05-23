#pragma once
#include "Engine/Renderer/VertexData.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Core/HLTime.h"
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

		HLAPI static Ref<Mesh> Create(const MeshData& data);

		HLAPI MeshData& GetMeshData() { return m_MeshData; }
		HLAPI void Update(Timestep timestep);
		HLAPI void UpdateMeshData(std::vector<Vertex>& data);
		HLAPI void UpdateMeshData();

		HLAPI void SetTexture(int32 type, Ref<Texture> texture);
		HLAPI MaterialRenderProperties &GetMaterialRenderProperties();
		HLAPI MaterialPhysicalProperties &GetMaterialPhysicsProperties();

		HLAPI Ref<VertexArray>& GetVertexArray() { return m_VertexArray; }

		Ref<Animation> m_Animation = nullptr;
		Ref<Material> m_Material;

		HLAPI static void Copy(const Ref<Mesh>& src, Ref<Mesh>& dest);
		HLAPI static Ref<Mesh> Copy(const Ref<Mesh>& src);
		HLAPI Ref<Mesh> Copy() const;

	private:
		Mesh(const MeshData& data)
			: m_MeshData(data) {}

		MeshData m_MeshData;

	private:
		Ref<VertexArray> m_VertexArray;
	};
}
