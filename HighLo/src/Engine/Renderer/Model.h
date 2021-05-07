#pragma once
#include "Mesh.h"
#include "Engine/Core/DataTypes/HLString.h"
#include <vector>
#include <initializer_list>

namespace highlo
{
	class Model
	{
	public:
		HLAPI Model() {}

		HLAPI Model(const std::initializer_list<Ref<Mesh>>& meshes)
			: m_Meshes(meshes) {}

		HLAPI Model(const std::vector<Ref<Mesh>>& meshes)
			: m_Meshes(meshes) {}

		HLAPI uint64 GetMeshCount()			const { return m_Meshes.size(); }
		HLAPI Ref<Mesh> GetMesh(uint64 idx)	const { return m_Meshes.at(idx); }

		HLAPI void SetTexture(int type, const Ref<Texture>& texture);

		HLString Name = "";
		glm::vec4 Color = glm::vec4(1, 1, 1, 1);

	private:
		std::vector<Ref<Mesh>> m_Meshes;
	};
}
