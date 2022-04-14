// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-14) initial release
//

#pragma once

#include "Engine/Graphics/Shaders/ShaderPreProcessor.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

namespace highlo
{
	class OpenGLShaderPreProcessor : public ShaderPreProcessor
	{
	public:

		virtual ShaderType PreProcessHeader(HLString &contents, bool &isGuarded, std::unordered_set<HLString> &specialMacros, const std::unordered_set<IncludeData> &includeData, const FileSystemPath &fullPath) override;
		virtual std::unordered_map<ShaderType, HLString> PreProcessShader(const HLString &source, std::unordered_set<HLString> &specialMacros, ShaderLanguage language = ShaderLanguage::GLSL) override;
	};
}

#endif // HIGHLO_API_OPENGL

