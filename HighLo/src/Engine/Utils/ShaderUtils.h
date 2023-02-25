// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-01-23) initial release
//

#pragma once

#include "Engine/Core/DataTypes/String.h"
#include "Engine/Graphics/Shaders/Shader.h"

namespace highlo::utils
{
	static ShaderLanguage ShaderLanguageFromExtension(const HLString &extension)
	{
		if (extension == "glsl")
			return ShaderLanguage::GLSL;

		if (extension == "hlsl")
			return ShaderLanguage::HLSL;

		return ShaderLanguage::None;
	}
}

