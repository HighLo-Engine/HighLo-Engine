// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.3 (2022-03-03) Added Lerp, Interpolation functions
//     - 1.2 (2021-10-21) Added ultra-fast DotProduct in SSE (source: https://stackoverflow.com/a/42924346/12873837)
//     - 1.1 (2021-10-01) Added ultra-fast FastSquareRoot function (source: https://www.youtube.com/watch?v=p8u_k2LIZyo)
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include "MathSSE.h"

#include "Engine/Core/Core.h"
#include "Engine/Core/DataTypes/String.h"
#include "Engine/Core/Log.h"

namespace highlo::Math
{
	static std::unordered_map<float, float> s_CachedSin;
	static std::unordered_map<float, float> s_CachedCos;

#define WORLD_FRONT_DIR glm::vec3(1, 0, 0)
#define WORLD_UP_DIR glm::vec3(0, 1, 0)
#define WORLD_SIDE_DIR glm::vec3(0, 0, 1)

#define CLAMP(x, minimum, maximum) std::min(std::max(x, minimum), maximum);

#define HL_PI 3.1415926535897932f
#define HL_TWO_PI 6.28318530718f
#define HL_PI_INV 0.31830988618f
#define HL_PI_OVER_2 1.57079632679f
#define HL_PI_OVER_4 0.78539816339f
#define HL_PI_HALF HL_PI_OVER_2
#define HL_ZERO_TOLERANCE 1e-6f

	HLAPI float BarryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);

	HLAPI std::ostream &operator<<(std::ostream &os, const glm::vec2 &vec);
	HLAPI std::ostream &operator<<(std::ostream &os, const glm::vec3 &vec);
	HLAPI std::ostream &operator<<(std::ostream &os, const glm::vec4 &vec);
	HLAPI std::ostream &operator<<(std::ostream &os, const glm::mat2 &mat);
	HLAPI std::ostream &operator<<(std::ostream &os, const glm::mat3 &mat);
	HLAPI std::ostream &operator<<(std::ostream &os, const glm::mat4 &mat);
	HLAPI HLString Vec2ToString(const glm::vec2 &v);
	HLAPI HLString Vec3ToString(const glm::vec3 &v);
	HLAPI HLString Vec4ToString(const glm::vec4 &v);

	/// <summary>
	/// A very quick algorithm to compute the inverse square root 
	/// @see: https://en.wikipedia.org/wiki/Fast_inverse_square_root
	/// </summary>
	/// <param name="value">The value to compute the inverse square root from.</param>
	/// <returns>Returns the inverse square root of a given value.</returns>
	HLAPI float FastInverseSquareRoot(float value);

	HLAPI glm::vec3 ScreenToWorldRay(glm::vec2 point, const glm::mat4 &view, const glm::mat4 &projection, const glm::vec2 &view_size, const glm::vec2 &view_offset = glm::vec2(0, 0));
	HLAPI glm::vec3 WorldToScreen(const glm::vec3 &point, const glm::mat4 &view_matrix, const glm::mat4 &projection, const glm::vec2 &view_size, const glm::vec2 &view_offset = glm::vec2(0, 0));

	HLAPI bool Decompose(const glm::mat4 &transform, glm::vec3 &translation, glm::vec3 &scale, glm::vec3 &rotation);

	HLAPI void CreateCacheSin(float cacheSize = 100.0f);
	HLAPI void CreateCacheCos(float cacheSize = 100.0f);

	HLAPI float Sin(float value);
	HLAPI float Cos(float value);

	HLAPI uint32 GetDigitCountOf(uint32 number);

	/// <summary>
	/// Checks if the value is in the specified range.
	/// </summary>
	template<typename T>
	HLAPI static bool IsInRange(const T value, const T min, const T max)
	{
		return value >= min && value <= max;
	}

	/// <summary>
	/// Checks if the value is not in the specified range.
	/// </summary>
	template<typename T>
	HLAPI static bool IsNotInRange(const T value, const T min, const T max)
	{
		return value < min || value > max;
	}

	/// <summary>
	/// Checks if the value is a power of two.
	/// </summary>
	/// <param name="value">The value to check against.</param>
	/// <returns>Returns true if the value is a power of two.</returns>
	HLAPI static bool IsPowerOfTwo(uint32 value)
	{
		return (value & value - 1) == 0;
	}

	/// <summary>
	/// Wrapper around standard pow function
	/// </summary>
	/// <param name="base">The base.</param>
	/// <param name="exponent">The Exponent.</param>
	/// <returns>Returns the powed value.</returns>
	HLAPI static HL_FORCE_INLINE float Pow(const float base, const float exponent)
	{
		return powf(base, exponent);
	}

	/// <summary>
	/// Clamps the value between 0 and 1 (inclusive)
	/// </summary>
	/// <param name="value">The value to clamp.</param>
	/// <returns>Returns the clamped value.</returns>
	template<typename T>
	HLAPI static T Saturate(const T value)
	{
		return (value < 0) ? 0 : (value < 1) ? value : 1;
	}

	/// <summary>
	/// Linear interpolation, scalar ranges from 0-1
	/// </summary>
	/// <param name="x">The x.</param>
	/// <param name="y">The y.</param>
	/// <param name="s">The scalar value.</param>
	/// <returns></returns>
	template<typename T, typename Scalar>
	HLAPI static T Lerp(const T &x, const T &y, const Scalar &s)
	{
		return (T)(x + s * (y - x));
	}

	/// <summary>
	/// Linear interpolation, scalar ranges from 0-1
	/// </summary>
	/// <param name="x">The x.</param>
	/// <param name="y">The y.</param>
	/// <param name="s">The scalar value.</param>
	/// <returns></returns>
	template<typename T>
	HLAPI static T LerpStable(const T &x, const T &y, double s)
	{
		return (T)(x * (1.0 - s) + y * s);
	}

	/// <summary>
	/// Linear interpolation, scalar ranges from 0-1
	/// </summary>
	/// <param name="x">The x.</param>
	/// <param name="y">The y.</param>
	/// <param name="s">The scalar value.</param>
	/// <returns></returns>
	template<typename T>
	HLAPI static T LerpStable(const T &x, const T &y, float s)
	{
		return (T)(x * (1.0f - s) + y * s);
	}

	/// <summary>
	/// Calculates the linear parameter t to produce the interpolation value within the range [x, y].
	/// </summary>
	/// <param name="x">The x.</param>
	/// <param name="y">The y.</param>
	/// <param name="s">The scalar value.</param>
	/// <returns></returns>
	template<typename T, typename Scalar>
	HLAPI static T InverseLerp(const T &x, const T &y, const Scalar &s)
	{
		if (x == y)
			return static_cast<T>(0);

		return Saturate<T>((s - x) / (y - x));
	}

	/// <summary>
	/// Calculates a smooth Hermite interpolation.
	/// @see: https://en.wikipedia.org/wiki/Smoothstep
	/// </summary>
	/// <param name="x">The value itself.</param>
	/// <returns>Returns a smooth Hermite interpolation.</returns>
	HLAPI static float SmoothStep(float x)
	{
		return x <= 0.0f ? 0.0f : x >= 1.0f ? 1.0f : x * x * (3.0f - 2.0f * x);
	}

	/// <summary>
	/// Calculates a smooth Hermite interpolation.
	/// @see: https://en.wikipedia.org/wiki/Smoothstep
	/// </summary>
	/// <param name="x">The value itself.</param>
	/// <param name="min">The minimum value of x.</param>
	/// <param name="max">The maximum value of x.</param>
	/// <returns>Returns a smooth Hermite interpolation.</returns>
	HLAPI static float SmoothStep(float x, float min, float max)
	{
		if (x < min)
			return 0.0f;

		if (x >= max)
			return 1.0f;

		const float fraction = (x - min) / (max - min);
		return fraction * fraction * (3.0f - 2.0f * fraction);
	}

	/// <summary>
	/// Calculates a smoother Hermite interpolation.
	/// @see: https://en.wikipedia.org/wiki/Smoothstep#Variations
	/// </summary>
	/// <param name="x">The value itself.</param>
	/// <returns>Returns a smoother variant of the Hermite interpolation.</returns>
	HLAPI static float SmootherStep(float x)
	{
		return x <= 0.0f ? 0.0f : x >= 1.0f ? 1.0f : x * x * x * (x * (x * 6.0f - 15.0f) + 10.0f);
	}

	/// <summary>
	/// Calculates a smoother Hermite interpolation.
	/// @see: https://en.wikipedia.org/wiki/Smoothstep#Variations
	/// </summary>
	/// <param name="x">The value itself.</param>
	/// <param name="min">The minimum value for x.</param>
	/// <param name="max">The maximum value for x.</param>
	/// <returns>Returns a smoother variant of the Hermite interpolation.</returns>
	HLAPI static float SmootherStep(float x, float min, float max)
	{
		if (x < min)
			return 0.0f;

		if (x >= max)
			return 1.0f;

		const float fraction = (x - min) / (max - min);
		return fraction * fraction * fraction * (fraction * (fraction * 6.0f - 15.0f) + 10.0f);
	}

	/// <summary>
	/// Remaps a given value from one range to another.
	/// </summary>
	/// <param name="value">The value to remap.</param>
	/// <param name="fromMin">The minimum value to remap from.</param>
	/// <param name="fromMax">The maximum value to remap from.</param>
	/// <param name="toMin">The minimum value to remap to.</param>
	/// <param name="toMax">The maximum value to remap to.</param>
	/// <returns>Returns the new mapped value, that was mapped from [fromMin, fromMax] to [toMin, toMax].</returns>
	HLAPI static float Remap(float value, float fromMin, float fromMax, float toMin, float toMax)
	{
		return (value - fromMin) / (fromMax - fromMin) * (toMax - toMin) + toMin;
	}

	/// <summary>
	/// Performs a cubic interpolation.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <typeparam name="Scalar"></typeparam>
	/// <param name="p0"></param>
	/// <param name="p1"></param>
	/// <param name="t0"></param>
	/// <param name="t1"></param>
	/// <param name="alpha"></param>
	/// <returns></returns>
	template<typename T, typename Scalar>
	HLAPI static HL_FORCE_INLINE T CubicInterpolation(const T &p0, const T &p1, const T &t0, const T &t1, const Scalar &alpha)
	{
		const float alpha1 = alpha * alpha;
		const float alpha2 = alpha1 * alpha;
		return (T)((2 * alpha2 - 3 * alpha1 + 1) * p0) + (alpha2 - 2 * alpha1 + alpha) * t0 + (alpha2 - alpha1) * t1 + (-2 * alpha2 + 3 * alpha1) * p1;
	}

	/// <summary>
	/// Interpolates between a and b, applying an ease-in function.
	/// </summary>
	/// <param name="a">The first value to interpolate.</param>
	/// <param name="b">The second value to interpolate.</param>
	/// <param name="alpha">The alpha value.</param>
	/// <param name="exponent">The exponent, controls the degree of the curve.</param>
	/// <returns>Returns the new value that represents an ease-in interpolation.</returns>
	template<typename T>
	HLAPI static HL_FORCE_INLINE T InterpolateEaseIn(const T &a, const T &b, float alpha, float exponent)
	{
		const float blend = Pow(alpha, exponent);
		return Lerp<T>(a, b, blend);
	}

	/// <summary>
	/// Interpolates between a and b, applying an ease-out function.
	/// </summary>
	/// <param name="a">The first value to interpolate.</param>
	/// <param name="b">The second value to interpolate.</param>
	/// <param name="alpha">The alpha value.</param>
	/// <param name="exponent">The exponent, controls the degree of the curve.</param>
	/// <returns>Returns the new value that represents an ease-out interpolation.</returns>
	template<typename T>
	HLAPI static HL_FORCE_INLINE T InterpolateEaseOut(const T &a, const T &b, float alpha, float exponent)
	{
		const float blend = 1.0f - Pow(1.0f - alpha, exponent);
		return Lerp<T>(a, b, blend);
	}

	/// <summary>
	/// Interpolates between a and b, applying an ease-in-out function.
	/// </summary>
	/// <param name="a">The first value to interpolate.</param>
	/// <param name="b">The second value to interpolate.</param>
	/// <param name="alpha">The alpha value.</param>
	/// <param name="exponent">The exponent, controls the degree of the curve.</param>
	/// <returns>Returns the new value that represents an ease-in-out interpolation.</returns>
	template<typename T>
	HLAPI static HL_FORCE_INLINE T InterpolateEaseInOut(const T &a, const T &b, float alpha, float exponent)
	{
		return Lerp<T>(a, b, alpha < 0.5f ? InterpolateEaseIn(0.0f, 1.0f, alpha * 2.0f, exponent) * 0.5f : InterpolateEaseOut(0.0f, 1.0f, alpha * 2.0f - 1.0f, exponent) * 0.5f + 0.5f);
	}

	/// <summary>
	/// Interpolation between a and b, applying a sinusoidal-in function.
	/// </summary>
	/// <param name="a">The first value to interpolate.</param>
	/// <param name="b">The second value to interpolate.</param>
	/// <param name="alpha">The alpha value.</param>
	/// <returns>Returns the new value that represents a sinusoidal-in interpolation.</returns>
	template<typename T>
	HLAPI static HL_FORCE_INLINE T InterpolateSinusoidalIn(const T &a, const T &b, float alpha)
	{
		const float blend = -1.0f * Cos(alpha * HL_PI_HALF) + 1.0f;
		return Lerp<T>(a, b, blend);
	}

	/// <summary>
	/// Interpolation between a and b, applying a sinusoidal-out function.
	/// </summary>
	/// <param name="a">The first value to interpolate.</param>
	/// <param name="b">The second value to interpolate.</param>
	/// <param name="alpha">The alpha value.</param>
	/// <returns>Returns the new value that represents a sinusoidal-out interpolation.</returns>
	template<typename T>
	HLAPI static HL_FORCE_INLINE T InterpolateSinusoidalOut(const T &a, const T &b, float alpha)
	{
		const float blend = Sin(alpha * HL_PI_HALF);
		return Lerp<T>(a, b, blend);
	}

	/// <summary>
	/// Interpolation between a and b, applying a sinusoidal-in-out function.
	/// </summary>
	/// <param name="a">The first value to interpolate.</param>
	/// <param name="b">The second value to interpolate.</param>
	/// <param name="alpha">The alpha value.</param>
	/// <returns>Returns the new value that represents a sinusoidal-in-out interpolation.</returns>
	template<typename T>
	HLAPI static HL_FORCE_INLINE T InterpolateSinusoidalInOut(const T &a, const T &b, float alpha)
	{
		return Lerp<T>(a, b, alpha < 0.5f ? InterpolateSinusoidalIn(0.0f, 1.0f, alpha * 2.0f) * 0.5f : InterpolateSinusoidalOut(0.0f, 1.0f, alpha * 2.0f - 1.0f) * 0.5f + 0.5f);
	}

	/// <summary>
	/// Interpolation between a and b, applying an exponential-in function.
	/// </summary>
	/// <param name="a">The first value to interpolate.</param>
	/// <param name="b">The second value to interpolate.</param>
	/// <param name="alpha">The alpha value.</param>
	/// <returns>Returns the new value that represents an exponential-in interpolation.</returns>
	template<typename T>
	HLAPI static HL_FORCE_INLINE T InterpolateExponentialIn(const T &a, const T &b, float alpha)
	{
		const float blend = alpha == 0.0f ? 0.0f : Pow(2.0f, 10.0f * (alpha - 1.0f));
		return Lerp<T>(a, b, blend);
	}

	/// <summary>
	/// Interpolation between a and b, applying an exponential-out function.
	/// </summary>
	/// <param name="a">The first value to interpolate.</param>
	/// <param name="b">The second value to interpolate.</param>
	/// <param name="alpha">The alpha value.</param>
	/// <returns>Returns the new value that represents an exponential-out interpolation.</returns>
	template<typename T>
	HLAPI static HL_FORCE_INLINE T InterpolateExponentialOut(const T &a, const T &b, float alpha)
	{
		const float blend = alpha == 1.0f ? 1.0f : -Pow(2.0f, -10.0f * alpha) + 1.0f;
		return Lerp<T>(a, b, blend);
	}

	/// <summary>
	/// Interpolation between a and b, applying an exponential-in-out function.
	/// </summary>
	/// <param name="a">The first value to interpolate.</param>
	/// <param name="b">The second value to interpolate.</param>
	/// <param name="alpha">The alpha value.</param>
	/// <returns>Returns the new value that represents an exponential-in-out interpolation.</returns>
	template<typename T>
	HLAPI static HL_FORCE_INLINE T InterpolateExponentialInOut(const T &a, const T &b, float alpha)
	{
		return Lerp<T>(a, b, alpha < 0.5f ? InterpolateExponentialIn(0.0f, 1.0f, alpha * 2.0f) * 0.5f : InterpolateExponentialOut(0.0f, 1.0f, alpha * 2.0f - 1.0f) * 0.5f + 0.5f);
	}

	/// <summary>
	/// Interpolation between a and b, applying a circular-in function.
	/// </summary>
	/// <param name="a">The first value to interpolate.</param>
	/// <param name="b">The second value to interpolate.</param>
	/// <param name="alpha">The alpha value.</param>
	/// <returns>Returns the new value that represents a circular-in interpolation.</returns>
	template<typename T>
	HLAPI static HL_FORCE_INLINE T InterpolateCircularIn(const T &a, const T &b, float alpha)
	{
		const float blend = -1.0f * (sqrtf(1.0f - alpha * alpha) - 1.0f);
		return Lerp<T>(a, b, blend);
	}

	/// <summary>
	/// Interpolation between a and b, applying a circular-out function.
	/// </summary>
	/// <param name="a">The first value to interpolate.</param>
	/// <param name="b">The second value to interpolate.</param>
	/// <param name="alpha">The alpha value.</param>
	/// <returns>Returns the new value that represents a circular-out interpolation.</returns>
	template<typename T>
	HLAPI static HL_FORCE_INLINE T InterpolateCircularOut(const T &a, const T &b, float alpha)
	{
		alpha -= 1.0f;
		const float blend = sqrtf(1.0f - alpha * alpha);
		return Lerp<T>(a, b, blend);
	}

	/// <summary>
	/// Interpolation between a and b, applying a circular-in-out function.
	/// </summary>
	/// <param name="a">The first value to interpolate.</param>
	/// <param name="b">The second value to interpolate.</param>
	/// <param name="alpha">The alpha value.</param>
	/// <returns>Returns the new value that represents a circular-in-out interpolation.</returns>
	template<typename T>
	HLAPI static HL_FORCE_INLINE T InterpolateCircularInOut(const T &a, const T &b, float alpha)
	{
		return Lerp<T>(a, b, alpha < 0.5f ? InterpolateCircularIn(0.0f, 1.0f, alpha * 2.0f) * 0.5f : InterpolateCircularOut(0.0f, 1.0f, alpha * 2.0f - 1.0f) * 0.5f + 0.5f);
	}


}

