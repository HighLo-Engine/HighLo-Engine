// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-26) initial release
//

#pragma once

#include <glm/glm.hpp>

namespace highlo
{
	struct Color
	{
		float R = 1.0f;
		float G = 1.0f;
		float B = 1.0f;
		float A = 1.0f;

		HLAPI Color() = default;
		HLAPI Color(float r, float g, float b, float a);
		HLAPI Color(const glm::vec3 &rgb);
		HLAPI Color(const glm::vec4 &rgba);
		HLAPI ~Color();

		HLAPI Color(const Color&) = default;
		HLAPI Color &operator=(const Color&) = default;

		HLAPI void ToSRGB();
		HLAPI void FromSRGB();

		HLAPI static Color ToSRGB(const Color &color);
		HLAPI static Color FromSRGB(const Color &color);

		HLAPI bool operator==(const Color &other) const;
		HLAPI bool operator!=(const Color &other) const;

		HLAPI operator glm::vec4() { return glm::vec4(R, G, B, A); }
		HLAPI operator const glm::vec4() const { return glm::vec4(R, G, B, A); }
	};
}

