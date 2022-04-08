// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "GLSLIncluder.h"

#include <libshaderc_util/io_shaderc.h>

namespace highlo
{
	namespace utils
	{
		static int32 SkipBOM(std::istream &in)
		{
			char test[4] = { 0 };
			in.seekg(0, std::ios::beg);
			in.read(test, 3);
			
			if (strcmp(test, "\xEF\xBB\xBF") == 0)
			{
				in.seekg(3, std::ios::beg);
				return 3;
			}

			in.seekg(0, std::ios::beg);
			return 0;
		}

		static HLString ReadFileAndSkipBOM(const std::filesystem::path &filePath)
		{
			HLString result;
			std::ifstream in(filePath, std::ios::in | std::ios::binary);
			if (in)
			{
				in.seekg(0, std::ios::end);
				uint32 fileSize = (uint32)in.tellg();
				const int32 skippedChars = SkipBOM(in);

				fileSize -= skippedChars - 1;
				result.Resize(fileSize);
				in.read(result.C_Str() + 1, fileSize);

				// Add a dummy tab to beginning of file.
				result[0] = '\t';
			}

			in.close();
			return result;
		}
	}

	GLSLIncluder::GLSLIncluder(const shaderc_util::FileFinder *fileFinder)
		: m_FileFinder(*fileFinder)
	{
		m_PreProcessor = ShaderPreProcessor::Create();
	}

	GLSLIncluder::~GLSLIncluder()
	{
		// Decrement the ref-count
		m_PreProcessor = nullptr;
	}

	shaderc_include_result *GLSLIncluder::GetInclude(const char *requestedPath, shaderc_include_type type, const char *requestingPath, size_t includeDepth)
	{
		std::filesystem::path requestedFullPath = (type == shaderc_include_type_relative) ? m_FileFinder.FindRelativeReadableFilepath(requestingPath, requestedPath) : m_FileFinder.FindReadableFilepath(requestedPath);
		auto &[source, sourceHash, shaderType, isGuarded] = m_HeaderCache[requestedFullPath.string()];

		if (source.IsEmpty())
		{
			source = utils::ReadFileAndSkipBOM(requestedFullPath);
			if (source.IsEmpty())
				HL_CORE_ERROR("Failed to load included file: {0} in {1}", requestedFullPath.string().c_str(), requestingPath);

			sourceHash = source.Hash();
			shaderType = m_PreProcessor->PreProcessHeader(source, isGuarded, m_ParsedSpecialMacros, m_IncludeData, FileSystemPath(requestedFullPath.string().c_str()));
		}
		else if (isGuarded)
		{
			source.Clear();
		}

		IncludeData inc;
		inc.IncludedFilePath = FileSystemPath(requestedFullPath.string().c_str());
		inc.IncludeDepth = includeDepth;
		inc.IsRelative = type == shaderc_include_type_relative;
		inc.IsGuarded = isGuarded;
		inc.HashValue = sourceHash;
		inc.IncludedType = shaderType;
		m_IncludeData.emplace(inc);

		auto *const container = new std::array<std::string, 2>;
		(*container)[0] = requestedPath;
		(*container)[1] = *source;

		auto *const data = new shaderc_include_result();
		data->user_data = container;

		data->source_name = (*container)[0].data();
		data->source_name_length = (*container)[0].size();

		data->content = (*container)[1].data();
		data->content_length = (*container)[1].size();

		return data;
	}

	void GLSLIncluder::ReleaseInclude(shaderc_include_result *result)
	{
		delete static_cast<std::array<std::string, 2>*>(result->user_data);
		delete result;
	}
}

