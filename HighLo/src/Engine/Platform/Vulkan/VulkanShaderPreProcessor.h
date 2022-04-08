#pragma once

#include "Engine/Graphics/Shaders/ShaderPreProcessor.h"

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>

namespace highlo
{
	class VulkanShaderPreProcessor : public ShaderPreProcessor
	{
	public:

		virtual ShaderType PreProcessHeader(HLString &contents, bool &isGuarded, std::unordered_set<HLString> &specialMacros, const std::unordered_set<IncludeData> &includeData, const FileSystemPath &fullPath) override;
		virtual std::unordered_map<ShaderType, HLString> PreProcessShader(const HLString &source, std::unordered_set<HLString> &specialMacros, ShaderLanguage language = ShaderLanguage::GLSL) override;
	};
}

#endif // HIGHLO_API_VULKAN

