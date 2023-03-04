// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-08) initial release
//

#pragma once

#include <shaderc/shaderc.hpp>
#include <libshaderc_util/file_finder.h>

#include "Shader.h"
#include "ShaderPreProcessor.h"

namespace highlo
{
	class GLSLIncluder : public shaderc::CompileOptions::IncluderInterface
	{
	public:

		HLAPI explicit GLSLIncluder(const shaderc_util::FileFinder *fileFinder);
		HLAPI ~GLSLIncluder() override;

		HLAPI shaderc_include_result *GetInclude(const char *requestedPath, shaderc_include_type type, const char *requestingPath, size_t includeDepth) override;
		HLAPI void ReleaseInclude(shaderc_include_result *result) override;

		HLAPI std::unordered_set<IncludeData> &&GetIncludeData() { return std::move(m_IncludeData); }
		HLAPI std::unordered_set<HLString> &&GetParsedSpecialMacros() { return std::move(m_ParsedSpecialMacros); }

	private:

		const shaderc_util::FileFinder &m_FileFinder;
		std::unordered_set<IncludeData> m_IncludeData;
		std::unordered_set<HLString> m_ParsedSpecialMacros;
		std::unordered_map<HLString, HeaderCache> m_HeaderCache;
	};
}

