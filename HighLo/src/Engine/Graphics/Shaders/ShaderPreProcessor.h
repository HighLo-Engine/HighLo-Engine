// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-07) initial release
//

#pragma once

#include "Shader.h"

namespace highlo
{
	struct IncludeData
	{
		FileSystemPath IncludedFilePath{};
		uint64 IncludeDepth = 0;
		bool IsRelative = false;
		bool IsGuarded = false;
		uint64 HashValue = 0;

		ShaderType IncludedType = ShaderType::None;

		bool operator==(const IncludeData &other) const
		{
			return IncludedFilePath == other.IncludedFilePath && HashValue == other.HashValue;
		}

		bool operator!=(const IncludeData &other) const
		{
			return !(*this == other);
		}
	};

	struct HeaderCache
	{
		HLString Source;
		uint64 SourceHash;
		ShaderType Type;
		bool IsGuarded;
	};
}

namespace std
{
	template<>
	struct hash<highlo::IncludeData>
	{
		uint64 operator()(const highlo::IncludeData &includeData) const noexcept
		{
			return includeData.IncludedFilePath.Hash() ^ includeData.HashValue;
		}
	};
}

namespace highlo
{
	struct StageData
	{
		std::unordered_set<IncludeData> Headers;
		uint64 HashValue = 0;

		bool operator==(const StageData &other) const
		{
			return Headers == other.Headers && HashValue == other.HashValue;
		}

		bool operator!=(const StageData &other) const
		{
			return !(*this == other);
		}
	};

	class ShaderPreProcessor : public IsSharedReference
	{
	public:

		HLAPI virtual ShaderType PreProcessHeader(HLString &contents, bool &isGuarded, std::unordered_set<HLString> &specialMacros, const std::unordered_set<IncludeData> &includeData, const FileSystemPath &fullPath) = 0;
		HLAPI virtual std::unordered_map<ShaderType, HLString> PreProcessShader(const HLString &source, std::unordered_set<HLString> &specialMacros, ShaderLanguage language = ShaderLanguage::GLSL) = 0;

		HLAPI static Ref<ShaderPreProcessor> Create();
	};
}

