#pragma once
#include "Engine/Renderer/VertexData.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Core/Time.h"
#include "Material.h"
#include "Animation.h"

namespace highlo
{
	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<int32>	Indices;
	};

	class Mesh : public IsSharedReference
	{
	public:

		HLAPI static Ref<Mesh> Create(const MeshData &data);

		HLAPI MeshData &GetMeshData() { return m_MeshData; }
		HLAPI void Update(Timestep timestep);
		HLAPI void UpdateMeshData(std::vector<Vertex> &data, uint32 meshIndex = 0);
		HLAPI void UpdateMeshData(uint32 meshIndex = 0);

		HLAPI void SetTexture(int32 type, Ref<Texture> texture);
		HLAPI MaterialRenderProperties &GetMaterialRenderProperties();
		HLAPI MaterialPhysicalProperties &GetMaterialPhysicsProperties();
		
		HLAPI bool HasAnimation() const { return m_Animation != nullptr; }
		HLAPI Ref<Animation> &GetAnimation() { return m_Animation; }
		HLAPI const Ref<Animation> &GetAnimation() const { return m_Animation; }
		HLAPI void SetAnimation(const Ref<Animation> &anim) { m_Animation = anim; }

		HLAPI bool HasMaterial() const { return m_Material != nullptr; }
		HLAPI Ref<Material> &GetMaterial() { return m_Material; }
		HLAPI const Ref<Material> &GetMaterial() const { return m_Material; }
		HLAPI void SetMaterial(const Ref<Material> &mat) { m_Material = mat; }

		HLAPI Ref<VertexArray> &GetVertexArray() { return m_VertexArray; }

		HLAPI static void Copy(const Ref<Mesh> &src, Ref<Mesh> &dest);
		HLAPI static Ref<Mesh> Copy(const Ref<Mesh> &src);
		HLAPI Ref<Mesh> Copy() const;

	private:
		Mesh(const MeshData &data)
			: m_MeshData(data) {}

		MeshData m_MeshData;
		Ref<VertexArray> m_VertexArray;

		Ref<Animation> m_Animation = nullptr;
		Ref<Material> m_Material;
	};
}
