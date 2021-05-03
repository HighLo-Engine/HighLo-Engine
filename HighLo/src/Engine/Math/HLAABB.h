#pragma once

#include <glm/glm.hpp>
#include <math.h>
#include <cmath>

#include "Engine/Core/HLCore.h"
#include "Engine/Core/HLLog.h"

namespace highlo
{
	struct AABB
	{
		glm::vec3 Min, Max;

		AABB()
			: Min(0.0f), Max(0.0f) {}

		AABB(const glm::vec3 &min, const glm::vec3 &max)
			: Min(min), Max(max)
		{
		}

		float Width() const
		{
			return Max.x - Min.x;
		}

		float Height() const
		{
			return Max.y - Min.y;
		}

		float Depth() const
		{
			return Max.z - Min.z;
		}

		float X() const
		{
			return (Max.x + Min.x) / 2;
		}

		float Y() const
		{
			return (Max.y + Min.y) / 2;
		}

		float Z() const
		{
			return (Max.z + Min.z) / 2;
		}

		bool IsEmpty() const
		{
			return Min.x >= Max.x || Min.y >= Max.y || Min.z >= Max.z;
		}

		void Clear()
		{
			Min = { MAX_FLOAT, MAX_FLOAT, MAX_FLOAT };
			Max = { MAX_FLOAT, MAX_FLOAT, MAX_FLOAT };
		}

		void Add(const glm::vec3 &coordinate)
		{
			Add(coordinate.x, coordinate.y, coordinate.z);
		}

		void Add(float x, float y, float z)
		{
			HL_ASSERT(!isinf(x) && !isnan(x));
			HL_ASSERT(!isinf(y) && !isnan(y));
			HL_ASSERT(!isinf(z) && !isnan(z));

			Min.x = HL_MIN(Min.x, std::floor(x));
			Min.y = HL_MIN(Min.y, std::floor(y));
			Min.z = HL_MIN(Min.z, std::floor(z));

			Max.x = HL_MAX(Max.x, std::ceil(x));
			Max.y = HL_MAX(Max.y, std::ceil(y));
			Max.z = HL_MAX(Max.z, std::ceil(z));
		}

		void Add(const AABB &other)
		{
			Min.x = HL_MIN(Min.x, other.Min.x);
			Min.y = HL_MIN(Min.y, other.Min.y);
			Min.z = HL_MIN(Min.z, other.Min.z);

			Max.x = HL_MAX(Max.x, other.Max.x);
			Max.y = HL_MAX(Max.y, other.Max.y);
			Max.z = HL_MAX(Max.z, other.Max.z);
		}

		void Set(const AABB &other)
		{
			Min.x = other.Min.x;
			Min.y = other.Min.y;
			Min.z = other.Min.z;

			Max.x = other.Max.x;
			Max.y = other.Max.y;
			Max.z = other.Max.z;
		}

		void Translate(float x, float y, float z)
		{
			Min.x += x;
			Min.y += y;
			Min.z += z;

			Max.x += x;
			Max.y += y;
			Max.z += z;
		}

		void Translate(const glm::vec3 &translation)
		{
			Min.x = std::floor(Min.x + translation.x);
			Min.y = std::floor(Min.y + translation.y);
			Min.z = std::floor(Min.z + translation.z);

			Max.x = std::ceil(Max.x + translation.x);
			Max.y = std::ceil(Max.y + translation.y);
			Max.z = std::ceil(Max.z + translation.z);
		}

		AABB Translated(const glm::vec3 &translation) const
		{
			return AABB({ Min.x + translation.x, Min.y + translation.y, Min.z + translation.z },
						{ Max.x + translation.x, Max.y + translation.y,	Max.z + translation.z });
		}

		void Set(const AABB &other, const glm::vec3 &translation)
		{
			Min.x = std::floor(other.Min.x + translation.x);
			Min.y = std::floor(other.Min.y + translation.y);
			Min.z = std::floor(other.Min.z + translation.z);

			Max.x = std::ceil(other.Max.x + translation.x);
			Max.y = std::ceil(other.Max.y + translation.y);
			Max.z = std::ceil(other.Max.z + translation.z);
		}

		bool IsOutside(const AABB &other) const
		{
			return Max.x - other.Min.x < 0 || Min.x - other.Max.x > 0 ||
				   Max.y - other.Min.y < 0 || Min.y - other.Max.y > 0 ||
				   Max.z - other.Min.z < 0 || Min.z - other.Max.z > 0;
		}

		static bool IsOutside(const AABB &left, const AABB &right)
		{
			return left.Max.x - right.Min.x < 0 || left.Min.x - right.Max.x > 0 ||
				   left.Max.y - right.Min.y < 0 || left.Min.y - right.Max.y > 0 ||
				   left.Max.z - right.Min.z < 0 || left.Min.z - right.Max.z > 0;
		}
	};
}