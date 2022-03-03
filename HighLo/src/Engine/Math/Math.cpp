// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Math.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Engine/Core/Profiler/ProfilerTimer.h"
#include "Engine/Window/Window.h"

namespace highlo::Math
{
	float BarryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos)
	{
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}

	std::ostream &operator<<(std::ostream &os, const glm::vec2 &vec)
	{
		return os << vec.x << ", " << vec.y;
	}

	std::ostream &operator<<(std::ostream &os, const glm::vec3 &vec)
	{
		return os << vec.x << ", " << vec.y << ", " << vec.z;
	}

	std::ostream &operator<<(std::ostream &os, const glm::vec4 &vec)
	{
		return os << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
	}

	std::ostream &operator<<(std::ostream &os, const glm::mat2 &mat)
	{
		for (uint32 i = 0; i < 2; ++i)
		{
			for (uint32 j = 0; j < 2; ++j)
			{
				os << mat[i][j] << " ";
			}
			os << "\n";
		}

		return os;
	}

	std::ostream &operator<<(std::ostream &os, const glm::mat3 &mat)
	{
		for (uint32 i = 0; i < 3; ++i)
		{
			for (uint32 j = 0; j < 3; ++j)
			{
				os << mat[i][j] << " ";
			}
			os << "\n";
		}

		return os;
	}

	std::ostream &operator<<(std::ostream &os, const glm::mat4 &mat)
	{
		for (uint32 i = 0; i < 4; ++i)
		{
			for (uint32 j = 0; j < 4; ++j)
			{
				os << mat[i][j] << " ";
			}
			os << "\n";
		}

		return os;
	}

	HLString Vec2ToString(const glm::vec2 &v)
	{
		return HLString::ToString(v.x) + ", " + HLString::ToString(v.y);
	}

	HLString Vec3ToString(const glm::vec3 &v)
	{
		return HLString::ToString(v.x) + ", " + HLString::ToString(v.y) + ", " + HLString::ToString(v.z);
	}

	HLString Vec4ToString(const glm::vec4 &v)
	{
		return HLString::ToString(v.x) + ", " + HLString::ToString(v.y) + ", " + HLString::ToString(v.z) + ", " + HLString::ToString(v.w);
	}

	float FastInverseSquareRoot(float value)
	{
		uint32 i;
		float x2, y;

		x2 = value * 0.5f;
		y = value;

		i = *(long*)&y;
		i = 0x5f3759df - (i >> 1);
		y = *(float*)&i;

		// Newton iteration
		y = y * (1.5f - (x2 * y * y));
		y = y * (1.5f - (x2 * y * y));
		y = y * (1.5f - (x2 * y * y));

		return y;
	}

	glm::vec3 ScreenToWorldRay(glm::vec2 point, const glm::mat4 &view, const glm::mat4 &projection, const glm::vec2 &viewSize, const glm::vec2 &viewOffset)
	{
		glm::vec2 normalizedDeviceCoords;
		normalizedDeviceCoords.x = (2.0f * point.x) / viewSize.x - 1.0f;
		normalizedDeviceCoords.y = -1.0f * ((2.0f * point.y) / viewSize.y - 1.0f);

		normalizedDeviceCoords -= viewOffset;

		glm::vec4 clipSpaceCoords = glm::vec4(normalizedDeviceCoords.x, normalizedDeviceCoords.y, -1.0f, 1.0f);

		glm::vec4 eyeSpaceCoords = glm::inverse(projection) * clipSpaceCoords;
		eyeSpaceCoords.z = -1.0f;
		eyeSpaceCoords.w = 0.0f;

		glm::vec4 worldCoords = glm::inverse(view) * eyeSpaceCoords;

		return glm::normalize(glm::vec3(worldCoords.x, worldCoords.y, worldCoords.z));
	}

	glm::vec3 WorldToScreen(const glm::vec3 &point, const glm::mat4 &view, const glm::mat4 &projection, const glm::vec2 &viewSize, const glm::vec2 &viewOffset)
	{
		glm::vec4 worldPosition = glm::vec4(point, 1.0f);
		glm::vec4 clipSpacePos = projection * (view * worldPosition);
		glm::vec3 ndcPos = glm::vec3(clipSpacePos.x, clipSpacePos.y, clipSpacePos.z) / clipSpacePos.w;

		glm::vec2 result = ((glm::vec2(ndcPos) + 1.0f) / 2.0f) * viewSize + viewOffset;
		return glm::vec3(result.x, result.y, clipSpacePos.z);
	}

	bool Decompose(const glm::mat4 &transform, glm::vec3 &translation, glm::vec3 &scale, glm::vec3 &rotation)
	{
		glm::mat4 localMatrix(transform);

		// normalize the matrix
		if (glm::epsilonEqual(localMatrix[3][3], 0.0f, glm::epsilon<float>()))
			return false;

		// First isolate the perspective
		if (glm::epsilonNotEqual(localMatrix[0][3], 0.0f, glm::epsilon<float>()) ||
			glm::epsilonNotEqual(localMatrix[1][3], 0.0f, glm::epsilon<float>()) ||
			glm::epsilonNotEqual(localMatrix[2][3], 0.0f, glm::epsilon<float>()))
		{
			localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = 0.0f;
			localMatrix[3][3] = 1.0f;
		}

		// Next take care of translation
		translation = glm::vec3(localMatrix[3]);
		localMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, localMatrix[3].w);

		// Now get the scale and shear
		glm::vec3 row[3];

		for (glm::length_t i = 0; i < 3; ++i)
			for (glm::length_t j = 0; j < 3; ++j)
				row[i][j] = localMatrix[i][j];

		// Compute X scale factor and normalize first row
		scale.x = glm::length(row[0]);
		row[0] = glm::detail::scale(row[0], 1.0f);
		
		scale.y = glm::length(row[1]);
		row[1] = glm::detail::scale(row[1], 1.0f);

		scale.z = glm::length(row[2]);
		row[2] = glm::detail::scale(row[2], 1.0f);

		// Get the rotation
		rotation.y = asin(-row[0][2]);
		if (cos(rotation.y) != 0)
		{
			rotation.x = atan2(row[1][2], row[2][2]);
			rotation.z = atan2(row[0][1], row[0][0]);
		}
		else
		{
			rotation.x = atan2(-row[2][0], row[1][1]);
			rotation.z = 0.0f;
		}

		return true;
	}
	
	void CreateCacheSin(float cacheSize)
	{
		// The idea is to pre-run a wide variation of values with the Sin function and store the results in some sort of table that the engine can look up during runtime
		// this would be much faster because sin() and cos() are expensive to calculate
		HL_PROFILE_FUNCTION();

		for (int32 i = 0; i < cacheSize; ++i)
		{
			float cachedValue = std::sin((float)(HL_PI * i / 1000.0f));
			s_CachedSin.insert({ (float)(HL_PI * i / 1000.0f), cachedValue });
		}
	}

	void CreateCacheCos(float cacheSize)
	{
		// The idea is to pre-run a wide variation of values with the Sin function and store the results in some sort of table that the engine can look up during runtime
		// this would be much faster because sin() and cos() are expensive to calculate
		HL_PROFILE_FUNCTION();

		for (int32 i = 0; i < cacheSize; ++i)
		{
			float cachedValue = std::cos((float)(HL_PI * i / 1000.0f));
			s_CachedCos.insert({ (float)(HL_PI * i / 1000.0f), cachedValue });
		}
	}
	
	float Sin(float value)
	{
		HL_PROFILE_FUNCTION();

		if (s_CachedSin.find(value) == s_CachedSin.end())
		{
			// Cache miss - value is not cached yet
			HL_WARN("Cache miss at sinus cache - caching value {0}", value);
			s_CachedSin.insert({ value, ::sin(value) });
		}

		return s_CachedSin[value];
	}
	
	float Cos(float value)
	{
		HL_PROFILE_FUNCTION();

		if (s_CachedCos.find(value) == s_CachedCos.end())
		{
			// cache miss - value is not cached yet
			HL_WARN("Cache miss at cosinus cache - caching value {0}", value);
			s_CachedCos.insert({ value, ::cos(value) });
		}

		return s_CachedCos[value];
	}
	
	uint32 GetDigitCountOf(uint32 number)
	{
		return (uint32)log10(number) + 1;
	}
}

