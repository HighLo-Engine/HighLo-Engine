#pragma once
#include <core/HLCore.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <ostream>

namespace highlo
{
#define WORLD_UP_DIR glm::vec3(0, 1, 0)
#define CLAMP(x, minval, maxval) std::min(std::max(x, minval), maxval);

	float barry_centric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);

	std::ostream& operator<<(std::ostream& os, const glm::vec3& vec);
	std::string vec3ToString(const glm::vec3& v);

	glm::vec3 ScreenToWorldRay(glm::vec2 point, const glm::mat4& view, const glm::mat4& projection, const glm::vec2& view_size, const glm::vec2& view_offset = glm::vec2(0, 0));
	glm::vec3 WorldToScreen(glm::vec3 point, const glm::mat4& view_matrix, const glm::mat4& projection, const glm::vec2& view_size, const glm::vec2& view_offset = glm::vec2(0, 0));
}