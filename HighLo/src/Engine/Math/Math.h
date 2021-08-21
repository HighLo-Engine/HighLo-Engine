#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include "Engine/Core/Core.h"
#include "Engine/Core/DataTypes/String.h"
#include "Engine/Core/Log.h"

namespace highlo
{
	static std::unordered_map<float, float> s_CachedSin;
	static std::unordered_map<float, float> s_CachedCos;

#define WORLD_UP_DIR glm::vec3(0, 1, 0)
#define CLAMP(x, minval, maxval) std::min(std::max(x, minval), maxval);

#define HL_PI 3.1415926535897932f
#define HL_TWO_PI 6.28318530718f
#define HL_PI_INV 0.31830988618f
#define HL_PI_OVER_2 1.57079632679f
#define HL_PI_OVER_4 0.78539816339f
#define HL_PI_HALF HL_PI_OVER_2

// The value for which all absolute numbers smaller than are considered equal to zero.
#define HL_ZERO_TOLERANCE 1e-6f

#define HL_INTERSECTING 0
#define HL_INSIDE		1
#define HL_OUTSIDE		2

	HLAPI float barry_centric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);

	HLAPI std::ostream& operator<<(std::ostream& os, const glm::vec3& vec);
	HLAPI std::ostream& operator<<(std::ostream& os, const glm::mat4& mat);
	HLAPI HLString vec3ToString(const glm::vec3& v);

	HLAPI glm::vec3 ScreenToWorldRay(glm::vec2 point, const glm::mat4& view, const glm::mat4& projection, const glm::vec2& view_size, const glm::vec2& view_offset = glm::vec2(0, 0));
	HLAPI glm::vec3 WorldToScreen(const glm::vec3& point, const glm::mat4& view_matrix, const glm::mat4& projection, const glm::vec2& view_size, const glm::vec2& view_offset = glm::vec2(0, 0));

	HLAPI bool Decompose(const glm::mat4 &transform, glm::vec3 &translation, glm::vec3 &scale, glm::vec3 &rotation);
	HLAPI bool DecomposeTranslation(const glm::mat4 &transform, glm::vec3 &translation);
	HLAPI bool DecomposeScale(const glm::mat4 &transform, glm::vec3 &scale);
	HLAPI bool DecomposeRotation(const glm::mat4 &transform, glm::vec3 &rotation);

	HLAPI void CreateCacheSin(float cacheSize = 100.0f);
	HLAPI void CreateCacheCos(float cacheSize = 100.0f);

	HLAPI float Sin(float value);
	HLAPI float Cos(float value);

	HLAPI void	MatrixMulSSE(const glm::mat4& A, const glm::mat4& B, glm::mat4& dest);
	HLAPI float DistanceSquaredSSE(const glm::vec3& A, const glm::vec3& B);
	HLAPI bool	CompareVectorsSSE(const glm::vec3& A, const glm::vec3& B);
	HLAPI float	DotProductSSE(const glm::vec3& A, const glm::vec3& B);
}
