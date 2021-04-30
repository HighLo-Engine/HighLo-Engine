#include "HighLoPch.h"
#include "HLMath.h"

#include <window/Window.h>

namespace highlo
{
	float barry_centric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos)
	{
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}

	std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
	{
		os << vec.x << ", " << vec.y << ", " << vec.z;
		return os;
	}

	std::string vec3ToString(const glm::vec3& v)
	{
		return std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z);
	}

	glm::vec3 ScreenToWorldRay(glm::vec2 point, const glm::mat4& view, const glm::mat4& projection, const glm::vec2& view_size, const glm::vec2& view_offset)
	{
		glm::vec2 normalized_device_coords;
		normalized_device_coords.x = (2.0f * point.x) / view_size.x - 1.0f;
		normalized_device_coords.y = -1.0f * ((2.0f * point.y) / view_size.y - 1.0f);

		normalized_device_coords -= view_offset;

		glm::vec4 clip_space_coords = glm::vec4(normalized_device_coords.x, normalized_device_coords.y, -1.0f, 1.0f);

		glm::vec4 eye_space_coords = glm::inverse(projection) * clip_space_coords;
		eye_space_coords.z = -1.0f;
		eye_space_coords.w = 0.0f;

		glm::vec4 world_coords = glm::inverse(view) * eye_space_coords;

		return glm::normalize(glm::vec3(world_coords.x, world_coords.y, world_coords.z));
	}

	glm::vec3 WorldToScreen(glm::vec3 point, const glm::mat4& view_matrix, const glm::mat4& projection, const glm::vec2& view_size, const glm::vec2& view_offset)
	{
		glm::vec4 WorldPosition = glm::vec4(point, 1.0f);
		glm::vec4 ClipSpacePos = projection * (view_matrix * WorldPosition);

		glm::vec3 NDCPos = glm::vec3(ClipSpacePos.x, ClipSpacePos.y, ClipSpacePos.z) / ClipSpacePos.w;

		glm::vec2 result = ((glm::vec2(NDCPos) + 1.0f) / 2.0f) * view_size + view_offset;

		return glm::vec3(result.x, result.y, ClipSpacePos.z);
	}
}
