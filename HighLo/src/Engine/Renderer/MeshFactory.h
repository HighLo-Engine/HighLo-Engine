#pragma once

#include <glm/glm.hpp>
#include "Mesh.h"

namespace highlo
{
	class MeshFactory
	{
	public:

		HLAPI static Ref<Mesh> CreateCube(const glm::vec3 &size);
		HLAPI static Ref<Mesh> CreateSphere(float radius);
		HLAPI static Ref<Mesh> CreateCapsule(float radius, float height);
	};
}

