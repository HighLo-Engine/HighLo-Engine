// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-11-02) Added Theme Colors
//     - 1.0 (2021-09-26) initial release
//

#pragma once

#include <glm/glm.hpp>

#include "Engine/ImGui/ImGui/imgui.h"

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

	namespace Colors::Theme
	{
		constexpr auto Accent = IM_COL32(236, 158, 36, 255);
		constexpr auto Highlight = IM_COL32(39, 185, 242, 255);
		constexpr auto NiceBlue = IM_COL32(83, 232, 254, 255);
		constexpr auto Compliment = IM_COL32(78, 151, 166, 255);
		constexpr auto Background = IM_COL32(36, 36, 36, 255);
		constexpr auto BackgroundDark = IM_COL32(26, 26, 26, 255);
		constexpr auto Titlebar = IM_COL32(21, 21, 21, 255);
		constexpr auto PropertyField = IM_COL32(15, 15, 15, 255);
		constexpr auto Text = IM_COL32(192, 192, 192, 255);
		constexpr auto TextBrighter = IM_COL32(210, 210, 210, 255);
		constexpr auto TextDarker = IM_COL32(128, 128, 128, 255);
		constexpr auto Muted = IM_COL32(77, 77, 77, 255);
		constexpr auto GroupHeader = IM_COL32(47, 47, 47, 255);
		constexpr auto Selection = IM_COL32(237, 192, 119, 255);
		constexpr auto SelectionMuted = IM_COL32(237, 201, 142, 23);
		constexpr auto BackgroundPopup = IM_COL32(63, 70, 77, 255); // most blue

	}
}

