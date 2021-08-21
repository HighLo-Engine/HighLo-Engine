#include "HighLoPch.h"
#include "Math.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Engine/Core/Profiler/ProfilerTimer.h"
#include "Engine/Window/Window.h"

#define SHUFFLE_PARAM(x, y, z, w) \
    (z<<6) | (y<<4) | (x<<2) | w//((x) | ((y) << 2) | ((z) << 4) | ((w) << 6))

#define _mm_madd_ps(a, b, c) \
    _mm_add_ps(_mm_mul_ps((a), (b)), (c))

#define _mm_replicate_x_ps(v) \
    _mm_shuffle_ps((v), (v), SHUFFLE_PARAM(0, 0, 0, 0))

#define _mm_replicate_y_ps(v) \
    _mm_shuffle_ps((v), (v), SHUFFLE_PARAM(1, 1, 1, 1))

#define _mm_replicate_z_ps(v) \
    _mm_shuffle_ps((v), (v), SHUFFLE_PARAM(2, 2, 2, 2))

#define _mm_replicate_w_ps(v) \
    _mm_shuffle_ps((v), (v), SHUFFLE_PARAM(3, 3, 3, 3))

namespace highlo
{
	float hsum_ps_sse3(__m128 v)
	{
		__m128 shuf = _mm_movehdup_ps(v);
		__m128 sums = _mm_add_ps(v, shuf);
		shuf = _mm_movehl_ps(shuf, sums);
		sums = _mm_add_ss(sums, shuf);
		return _mm_cvtss_f32(sums);
	}

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

	std::ostream& operator<<(std::ostream& os, const glm::mat4& mat)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				os << mat[i][j] << " ";
			}
			os << "\n";
		}
		return os;
	}

	HLString vec3ToString(const glm::vec3& v)
	{
		return HLString::ToString(v.x) + ", " + HLString::ToString(v.y) + ", " + HLString::ToString(v.z);
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

	glm::vec3 WorldToScreen(const glm::vec3& point, const glm::mat4& view_matrix, const glm::mat4& projection, const glm::vec2& view_size, const glm::vec2& view_offset)
	{
		glm::vec4 WorldPosition = glm::vec4(point, 1.0f);
		glm::vec4 ClipSpacePos = projection * (view_matrix * WorldPosition);

		glm::vec3 NDCPos = glm::vec3(ClipSpacePos.x, ClipSpacePos.y, ClipSpacePos.z) / ClipSpacePos.w;

		glm::vec2 result = ((glm::vec2(NDCPos) + 1.0f) / 2.0f) * view_size + view_offset;

		return glm::vec3(result.x, result.y, ClipSpacePos.z);
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
			localMatrix[0][3] = localMatrix[1][3] = 0.0f;
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

	bool DecomposeTranslation(const glm::mat4 &transform, glm::vec3 &translation)
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
			localMatrix[0][3] = localMatrix[1][3] = 0.0f;
			localMatrix[3][3] = 1.0f;
		}

		// Next take care of translation
		translation = glm::vec3(localMatrix[3]);
		localMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, localMatrix[3].w);

		return false;
	}

	bool DecomposeScale(const glm::mat4 &transform, glm::vec3 &scale)
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
			localMatrix[0][3] = localMatrix[1][3] = 0.0f;
			localMatrix[3][3] = 1.0f;
		}

		// Next take care of translation - just unset it in the matrix because the user does not want to know about it
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

		return false;
	}

	bool DecomposeRotation(const glm::mat4 &transform, glm::vec3 &rotation)
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
			localMatrix[0][3] = localMatrix[1][3] = 0.0f;
			localMatrix[3][3] = 1.0f;
		}

		// Next take care of translation - just unset it in the matrix because the user does not want to know about it
		localMatrix[3] = glm::vec4(0.0f, 0.0f, 0.0f, localMatrix[3].w);

		// Now get the scale and shear
		glm::vec3 row[3];

		// Even though the transform is a 4x4 matrix, we don't really need the fourth dimension upon this point
		for (glm::length_t i = 0; i < 3; ++i)
			for (glm::length_t j = 0; j < 3; ++j)
				row[i][j] = localMatrix[i][j];

		// Compute X scale factor and normalize first row - just unset it in the matrix because the user does not want to know about it
		row[0] = glm::detail::scale(row[0], 1.0f);
		row[1] = glm::detail::scale(row[1], 1.0f);
		row[2] = glm::detail::scale(row[2], 1.0f);

		// Get the rotation
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

		return false;
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
	
	void MatrixMulSSE(const glm::mat4& A, const glm::mat4& B, glm::mat4& dest)
	{
		__m128 m2_row_0 = _mm_setr_ps(A[0][0], A[0][1], A[0][2], A[0][3]);
		__m128 m2_row_1 = _mm_setr_ps(A[1][0], A[1][1], A[1][2], A[1][3]);
		__m128 m2_row_2 = _mm_setr_ps(A[2][0], A[2][1], A[2][2], A[2][3]);
		__m128 m2_row_3 = _mm_setr_ps(A[3][0], A[3][1], A[3][2], A[3][3]);

		__m128 m1_row_0 = _mm_setr_ps(B[0][0], B[0][1], B[0][2], B[0][3]);
		__m128 m1_row_1 = _mm_setr_ps(B[1][0], B[1][1], B[1][2], B[1][3]);
		__m128 m1_row_2 = _mm_setr_ps(B[2][0], B[2][1], B[2][2], B[2][3]);
		__m128 m1_row_3 = _mm_setr_ps(B[3][0], B[3][1], B[3][2], B[3][3]);

		__m128 out0;
		__m128 out1;
		__m128 out2;
		__m128 out3;

		out0 = _mm_mul_ps(m2_row_0, _mm_replicate_x_ps(m1_row_0));
		out1 = _mm_mul_ps(m2_row_0, _mm_replicate_x_ps(m1_row_1));
		out2 = _mm_mul_ps(m2_row_0, _mm_replicate_x_ps(m1_row_2));
		out3 = _mm_mul_ps(m2_row_0, _mm_replicate_x_ps(m1_row_3));

		out0 = _mm_madd_ps(m2_row_1, _mm_replicate_y_ps(m1_row_0), out0);
		out1 = _mm_madd_ps(m2_row_1, _mm_replicate_y_ps(m1_row_1), out1);
		out2 = _mm_madd_ps(m2_row_1, _mm_replicate_y_ps(m1_row_2), out2);
		out3 = _mm_madd_ps(m2_row_1, _mm_replicate_y_ps(m1_row_3), out3);

		out0 = _mm_madd_ps(m2_row_2, _mm_replicate_z_ps(m1_row_0), out0);
		out1 = _mm_madd_ps(m2_row_2, _mm_replicate_z_ps(m1_row_1), out1);
		out2 = _mm_madd_ps(m2_row_2, _mm_replicate_z_ps(m1_row_2), out2);
		out3 = _mm_madd_ps(m2_row_2, _mm_replicate_z_ps(m1_row_3), out3);

		out0 = _mm_madd_ps(m2_row_3, _mm_replicate_w_ps(m1_row_0), out0);
		out1 = _mm_madd_ps(m2_row_3, _mm_replicate_w_ps(m1_row_1), out1);
		out2 = _mm_madd_ps(m2_row_3, _mm_replicate_w_ps(m1_row_2), out2);
		out3 = _mm_madd_ps(m2_row_3, _mm_replicate_w_ps(m1_row_3), out3);

		_mm_store_ps(&dest[0][0], out0);
		_mm_store_ps(&dest[1][0], out1);
		_mm_store_ps(&dest[2][0], out2);
		_mm_store_ps(&dest[3][0], out3);
	}
	
	float DistanceSquaredSSE(const glm::vec3& A, const glm::vec3& B)
	{
		__m128 a4 = _mm_set_ps(A.x, A.y, A.z, 0.0f);
		__m128 b4 = _mm_set_ps(B.x, B.y, B.z, 0.0f);
		
		__m128 diff = _mm_sub_ps(b4, a4);
		__m128 sqr = _mm_mul_ps(diff, diff);

		return hsum_ps_sse3(sqr);
	}
	
	bool CompareVectorsSSE(const glm::vec3& A, const glm::vec3& B)
	{
		__m128i v1 = _mm_load_si128((__m128i*)&A);
		__m128i v2 = _mm_load_si128((__m128i*)&B);
		__m128i vcmp = _mm_cmpeq_epi32(v1, v2);
		uint16_t mask = _mm_movemask_epi8(vcmp);
		return (mask == 0xffff);
	}
	
	float DotProductSSE(const glm::vec3& A, const glm::vec3& B)
	{
		// TODO
		return 0;
	}
}