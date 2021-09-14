// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Mesh.h"
#include "Engine/Core/DataTypes/String.h"
#include "Engine/Math/AABB.h"
#include <vector>
#include <initializer_list>

namespace highlo
{
	class Model : public IsSharedReference
	{
	public:
		HLAPI Model() {}

		HLAPI Model(const std::initializer_list<Ref<Mesh>> &meshes)
			: m_Meshes(meshes) {}

		HLAPI Model(const std::vector<Ref<Mesh>> &meshes)
			: m_Meshes(meshes) {}

		HLAPI uint64 GetMeshCount()	const { return m_Meshes.size(); }
		HLAPI Ref<Mesh> GetMesh(uint64 idx)	const;

		HLString Name = "";
		glm::vec4 Color = glm::vec4(1, 1, 1, 1);
		AABB BoundingBox;

		HLAPI static Ref<Model> Create();
		HLAPI static Ref<Model> Create(const std::initializer_list<Ref<Mesh>>& meshes);
		HLAPI static Ref<Model> Create(const std::vector<Ref<Mesh>>& meshes);

	private:
		std::vector<Ref<Mesh>> m_Meshes;
	};
}
