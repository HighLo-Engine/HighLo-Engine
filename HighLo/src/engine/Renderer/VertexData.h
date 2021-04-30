#pragma once

#include "Engine/Core/HLCore.h"
#include "Engine/Math/HLMath.h"

namespace highlo
{
	struct Vertex
	{
		glm::vec3 position = glm::vec3(0, 0, 0);
		glm::vec2 uv = glm::vec2(0, 0);
		glm::vec3 normal = glm::vec3(0, 0, 0);
		glm::vec3 tangent = glm::vec3(0, 0, 0);
		glm::vec3 binormal = glm::vec3(0, 0, 0);
		glm::ivec4 jointIDs = glm::ivec4(-1, -1, -1, -1);
		glm::vec4 weights = glm::vec4(0, 0, 0, 0);

		Vertex() {}

		Vertex(const glm::vec3& position, const glm::vec2& uv, const glm::vec3& normal, const glm::vec3& tangent = glm::vec3(0, 0, 0), const glm::vec3& binormal = glm::vec3(0, 0, 0),
			const glm::ivec4& jointIDs = glm::ivec4(-1, -1, -1, -1), const glm::vec4& joint_weights = glm::vec4(0, 0, 0, 0))
			: position(position), uv(uv), normal(normal), tangent(tangent), binormal(binormal), jointIDs(jointIDs), weights(joint_weights) {}

		bool operator==(const Vertex& rhs)
		{
			return	position == rhs.position &&
				uv == rhs.uv &&
				normal == rhs.normal &&
				tangent == rhs.tangent &&
				binormal == rhs.binormal &&
				jointIDs == rhs.jointIDs &&
				weights == rhs.weights;
		}
	};
}
