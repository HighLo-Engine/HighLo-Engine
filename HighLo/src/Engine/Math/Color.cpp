// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Color.h"

namespace highlo
{
	namespace utils
	{
		static inline float ConvertSRGBFromLinear(float value)
		{
			return value <= 0.0031308f ? value * 12.92f : powf(value, 1.0f / 2.2f) * 1.055f - 0.055f;
		}

		static inline float ConvertSRGBToLinear(float value)
		{
			return value <= 0.04045f ? value / 12.92f : powf((value + 0.055f) / 1.055f, 2.2f);
		}
	}

	Color::Color(float r, float g, float b, float a)
		: R(r), G(g), B(b), A(a)
	{
	}
	
	Color::Color(const glm::vec3 &rgb)
		: R(rgb.x), G(rgb.y), B(rgb.z), A(1.0f)
	{
	}
	
	Color::Color(const glm::vec4 &rgba)
		: R(rgba.x), G(rgba.y), B(rgba.z), A(rgba.w)
	{
	}
	
	Color::~Color()
	{
	}
	
	void Color::ToSRGB()
	{
		R = std::pow(R, 2.2f);
		G = std::pow(G, 2.2f);
		B = std::pow(B, 2.2f);
	}

	void Color::FromSRGB()
	{
		R = utils::ConvertSRGBFromLinear(R);
		G = utils::ConvertSRGBFromLinear(G);
		B = utils::ConvertSRGBFromLinear(B);
	}

	Color Color::ToSRGB(const Color &color)
	{
		Color result;
		result.R = std::pow(color.R, 2.2f);
		result.G = std::pow(color.G, 2.2f);
		result.B = std::pow(color.B, 2.2f);
		result.A = color.A;
		return result;
	}
	
	Color Color::FromSRGB(const Color &color)
	{
		Color result;
		result.R = utils::ConvertSRGBFromLinear(color.R);
		result.G = utils::ConvertSRGBFromLinear(color.G);
		result.B = utils::ConvertSRGBFromLinear(color.B);
		result.A = color.A;
		return result;
	}
	
	bool Color::operator==(const Color &other) const
	{
		return R == other.R && G == other.G && B == other.B && A == other.A;
	}

	bool Color::operator!=(const Color &other) const
	{
		return !(*this == other);
	}
}

