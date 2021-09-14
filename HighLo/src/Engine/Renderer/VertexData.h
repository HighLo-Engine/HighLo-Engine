// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Math/Math.h"

namespace highlo
{
	struct Vertex
	{
		glm::vec3 Position = glm::vec3(0, 0, 0);
		glm::vec2 UV = glm::vec2(0, 0);
		glm::vec3 Normal = glm::vec3(0, 0, 0);
		glm::vec3 Tangent = glm::vec3(0, 0, 0);
		glm::vec3 Binormal = glm::vec3(0, 0, 0);
		glm::ivec4 JointIDs = glm::ivec4(-1, -1, -1, -1);
		glm::vec4 Weights = glm::vec4(0, 0, 0, 0);

		Vertex() {}

		Vertex(const glm::vec3& position, const glm::vec2& uv, const glm::vec3& normal, const glm::vec3& tangent = glm::vec3(0, 0, 0), const glm::vec3& binormal = glm::vec3(0, 0, 0),
			const glm::ivec4& jointIDs = glm::ivec4(-1, -1, -1, -1), const glm::vec4& joint_weights = glm::vec4(0, 0, 0, 0))
			: Position(position), UV(uv), Normal(normal), Tangent(tangent), Binormal(binormal), JointIDs(jointIDs), Weights(joint_weights) {}

		bool operator==(const Vertex &other) const
		{
			return Position == other.Position &&
				UV == other.UV &&
				Normal == other.Normal &&
				Tangent == other.Tangent &&
				Binormal == other.Binormal &&
				JointIDs == other.JointIDs &&
				Weights == other.Weights;
		}

		bool operator!=(const Vertex &other) const
		{
			return !(*this == other);
		}
	};
}
