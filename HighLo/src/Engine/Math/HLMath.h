#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Engine/Core/HLCore.h"
#include "Engine/Core/DataTypes/HLString.h"
#include "Engine/Core/HLLog.h"

namespace highlo
{
#define WORLD_UP_DIR glm::vec3(0, 1, 0)
#define CLAMP(x, minval, maxval) std::min(std::max(x, minval), maxval);

	float barry_centric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);

	std::ostream& operator<<(std::ostream& os, const glm::vec3& vec);
	HLString vec3ToString(const glm::vec3& v);

	glm::vec3 ScreenToWorldRay(glm::vec2 point, const glm::mat4& view, const glm::mat4& projection, const glm::vec2& view_size, const glm::vec2& view_offset = glm::vec2(0, 0));
	glm::vec3 WorldToScreen(glm::vec3 point, const glm::mat4& view_matrix, const glm::mat4& projection, const glm::vec2& view_size, const glm::vec2& view_offset = glm::vec2(0, 0));

	bool Decompose(const glm::mat4 &transform, glm::vec3 &translation, glm::vec3 &scale, glm::vec3 &rotation);
	bool DecomposeTranslation(const glm::mat4 &transform, glm::vec3 &translation);
	bool DecomposeScale(const glm::mat4 &transform, glm::vec3 &scale);
	bool DecomposeRotation(const glm::mat4 &transform, glm::vec3 &rotation);

	void CreateCacheSin();
	void CreateCacheCos();

	float Sin(float value);
	float Cos(float value);
}
