// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-04-07) initial release
//

#pragma once

#include "Shader.h"

#include <regex>

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

	namespace utils
	{
		static std::vector<std::string> SplitStringsAndKeepDelimiters(const std::string &str)
		{
			const static std::regex re(R"((^\W|^\w+)|(\w+)|[:()])", std::regex_constants::optimize);

			std::regex_iterator<std::string::const_iterator> rit(str.begin(), str.end(), re);
			std::regex_iterator<std::string::const_iterator> rend;
			std::vector<std::string> result;

			while (rit != rend)
			{
				result.emplace_back(rit->str());
				++rit;
			}

			return result;
		}

		template<bool RemoveHeaderGuard = false>
		bool ContainsHeaderGuard(std::string &header)
		{
			uint64 pos = header.find('#');
			while (pos != std::string::npos)
			{
				const uint64 endOfLine = header.find_first_of("\r\n", pos) + 1;
				auto tokens = utils::SplitStringsAndKeepDelimiters(header.substr(pos, endOfLine - pos));
				auto it = tokens.begin();

				if (*(++it) == "pragma")
				{
					if (*(++it) == "once")
					{
						if constexpr (RemoveHeaderGuard)
							header.erase(pos, endOfLine - pos);
						return true;
					}
				}
				pos = header.find('#', pos + 1);
			}
			return false;
		}

		// From https://wandbox.org/permlink/iXC7DWaU8Tk8jrf3
		enum class State : char
		{
			SlashOC,
			StarIC,
			SingleLineComment,
			MultiLineComment,
			NotAComment
		};

		template<typename InputIt, typename OutputIt>
		void CopyWithoutComments(InputIt first, InputIt last, OutputIt out)
		{
			State state = State::NotAComment;

			while (first != last)
			{
				switch (state)
				{
					case State::SlashOC:
					{
						if (*first == '/')
						{
							state = State::SingleLineComment;
						}
						else if (*first == '*')
						{
							state = State::MultiLineComment;
						}
						else
						{
							state = State::NotAComment;
							*out++ = '/';
							*out++ = *first;
						}

						break;
					}

					case State::StarIC:
					{
						if (*first == '/')
						{
							state = State::NotAComment;
						}
						else
						{
							state = State::MultiLineComment;
						}

						break;
					}

					case State::NotAComment:
					{
						if (*first == '/')
						{
							state = State::SlashOC;
						}
						else
						{
							*out++ = *first;
						}
						break;
					}

					case State::SingleLineComment:
					{
						if (*first == '\n')
						{
							state = State::NotAComment;
							*out++ = '\n';
						}
						break;
					}

					case State::MultiLineComment:
					{
						if (*first == '*')
						{
							state = State::StarIC;
						}
						else if (*first == '\n')
						{
							*out++ = '\n';
						}
						break;
					}
				}

				++first;
			}
		}

		static ShaderType ShaderTypeFromString(const std::string &str)
		{
			if (str == "vertex") return ShaderType::Vertex;
			if (str == "fragment") return ShaderType::Fragment;
			if (str == "compute") return ShaderType::Compute;
			if (str == "tess_control") return ShaderType::TessControl;
			if (str == "tess_eval") return ShaderType::TessEvaluation;
			if (str == "geometry") return ShaderType::Geometry;

			HL_ASSERT(false);
			return ShaderType::None;
		}

		static std::string_view StageToShaderMacro(const std::string_view stage)
		{
			if (stage == "vertex") return "__VERTEX_STAGE__";
			if (stage == "fragment") return "__FRAGMENT_STAGE__";
			if (stage == "compute") return "__COMPUTE_STAGE__";
			if (stage == "geometry") return "__GEOMETRY_STAGE__";
			if (stage == "tesscontrol") return "__TESS_CONTROL_STAGE__";
			if (stage == "tesseval") return "__TESS_EVAL_STAGE__";

			HL_ASSERT(false, "Unknown shader stage.");
			return "";
		}
	}

	class ShaderPreProcessor : public IsSharedReference
	{
	public:

		template<ShaderLanguage Lang>
		HLAPI static ShaderType PreprocessHeader(HLString &contents, bool &isGuarded, std::unordered_set<HLString> &specialMacros, const std::unordered_set<IncludeData> &includeData, const FileSystemPath &fullPath);

		template<ShaderLanguage Lang>
		HLAPI static std::unordered_map<ShaderType, HLString> PreprocessShader(const HLString &source, std::unordered_set<HLString> &specialMacros);
	};

	template<ShaderLanguage Lang>
	ShaderType ShaderPreProcessor::PreprocessHeader(HLString &contents, bool &isGuarded, std::unordered_set<HLString> &specialMacros, const std::unordered_set<IncludeData> &includeData, const FileSystemPath &fullPath)
	{
		std::stringstream sourceStream;
		std::string tmp = *contents;
		utils::CopyWithoutComments(tmp.begin(), tmp.end(), std::ostream_iterator<char>(sourceStream));
		std::string newSource = sourceStream.str();

		ShaderType stagesInHeader = ShaderType::None;

		isGuarded = utils::ContainsHeaderGuard<true>(newSource);

		uint32 stageCount = 0;
		uint64 startOfShaderStage = newSource.find('#', 0);

		while (startOfShaderStage != std::string::npos)
		{
			const uint64 endOfLine = newSource.find_first_of("\r\n", startOfShaderStage) + 1;
			const auto &tokens = utils::SplitStringsAndKeepDelimiters(newSource.substr(startOfShaderStage, endOfLine - startOfShaderStage));

			uint32 index = 0;
			if (tokens[index] == "#")
			{
				++index;

				if (index >= tokens.size())
				{
					HL_CORE_WARN("Could not find any pre-processor directive!");
					break;
				}

				++index; // Skip the pragma directive
				if (tokens[index] == "shader")
				{
					HL_ASSERT(tokens[index + 1] == ":");

					const std::string_view stage(tokens[index + 2]);
					HL_ASSERT(stage == "vertex" || stage == "fragment" || stage == "pixel" || stage == "geometry" || stage == "compute" || stage == "tess_control" || stage == "tess_eval", "Invalid shader type");
					ShaderType foundStage = utils::ShaderTypeFromString(std::string(stage.begin(), stage.end()));

					const bool alreadyIncluded = std::find_if(includeData.begin(), includeData.end(), [fullPath](const IncludeData &data)
					{
						return fullPath == data.IncludedFilePath;
					}) != includeData.end();

					if (isGuarded && alreadyIncluded)
					{
						newSource.clear();
					}
					else if (!isGuarded && alreadyIncluded)
					{
						HL_CORE_WARN("\"{0}\" Header does not contain a header guard (#pragma once).", **fullPath);
					}

					if (stageCount == 0)
					{
						newSource.replace(startOfShaderStage, endOfLine - startOfShaderStage, fmt::format("#ifdef {}", utils::StageToShaderMacro(stage)));
					}
					else
					{
						newSource.replace(startOfShaderStage, endOfLine - startOfShaderStage, fmt::format("#endif\r#ifdef {}", utils::StageToShaderMacro(stage)));
					}

					*(int32*)&stagesInHeader |= (int32)foundStage;
					++stageCount;
				}
				else if (tokens[index] == "ifdef")
				{
					++index;
					if (tokens[index].rfind("__HL_", 0) == 0)
					{
						specialMacros.emplace(tokens[index]);
					}
				}
				else if (tokens[index] == "if" || tokens[index] == "define")
				{
					++index;
					for (uint64 i = 0; i < tokens.size(); ++i)
					{
						if (tokens[index].rfind("__HL_", 0) == 0)
						{
							specialMacros.emplace(tokens[index]);
						}
					}
				}
			}

			startOfShaderStage = newSource.find('#', startOfShaderStage + 1);
		}

		if (stageCount)
		{
			newSource.append("\n#endif");
		}
		else
		{
			const bool alreadyIncluded = std::find_if(includeData.begin(), includeData.end(), [fullPath](const IncludeData &data)
			{
				return data.IncludedFilePath == fullPath;
			}) != includeData.end();

			if (isGuarded && alreadyIncluded)
			{
				newSource.clear();
			}
			else if (!isGuarded && alreadyIncluded)
			{
				HL_CORE_WARN("{0} Header does not contain a header guard (#pragma once)", **fullPath);
			}
		}

		contents = newSource;
		return stagesInHeader;
	}

	template<ShaderLanguage Lang>
	std::unordered_map<ShaderType, HLString> ShaderPreProcessor::PreprocessShader(const HLString &source, std::unordered_set<HLString> &specialMacros)
	{
		std::stringstream sourceStream;
		std::string tmp = *source;
		utils::CopyWithoutComments(tmp.begin(), tmp.end(), std::ostream_iterator<char>(sourceStream));
		std::string newSource = sourceStream.str();

		std::unordered_map<ShaderType, HLString> shaderSources;
		std::vector<std::pair<ShaderType, uint64>> stagePositions;
		HL_ASSERT(newSource.size(), "Shader is empty!");

		uint64 startOfStage = 0;
		uint64 pos = newSource.find('#');
		if (pos == std::string::npos)
		{
			HL_CORE_FATAL("[-] Could not find any '#' symbol. Aborting. [-]");
			return shaderSources;
		}

		// Check first #version
		if (Lang == ShaderLanguage::GLSL)
		{
			const uint64 endOfLine = newSource.find_first_of("\r\n", pos) + 1;
			const std::vector<std::string> &tokens = utils::SplitStringsAndKeepDelimiters(newSource.substr(pos, endOfLine - pos));
			HL_ASSERT(tokens.size() >= 3 && tokens[1] == "version", "Invalid #version encountered or #version is NOT encounted first.");
			pos = newSource.find('#', pos + 1);
		}

		while (pos != std::string::npos)
		{
			const uint64 endOfLine = newSource.find_first_of("\r\n", pos) + 1;
			const std::vector<std::string> &tokens = utils::SplitStringsAndKeepDelimiters(newSource.substr(pos, endOfLine - pos));

			uint64 index = 1; // Skip #
			if (tokens[index] == "pragma") // Parse stage. example: #pragma stage : vert
			{
				++index;
				if (tokens[index] == "shader")
				{
					++index;
					// Jump over ':'
					HL_ASSERT(tokens[index] == ":", "Shader pragma is invalid");
					++index;

					const std::string_view stage = tokens[index];
					HL_ASSERT(stage == "vertex" || stage == "fragment" || stage == "geometry" || stage == "compute" || stage == "tess_control" || stage == "tess_eval", "Invalid shader type");
					ShaderType shaderStage = utils::ShaderTypeFromString(std::string(stage.begin(), stage.end()));

					stagePositions.emplace_back(shaderStage, startOfStage);
				}
			}
			else if (tokens[index] == "ifdef")
			{
				++index;
				if (tokens[index].rfind("__HIGHLO_", 0) == 0)
				{
					specialMacros.emplace(tokens[index]);
				}
			}
			else if (tokens[index] == "if" || tokens[index] == "define")
			{
				++index;
				for (uint64 i = index; i < tokens.size(); ++i)
				{
					if (tokens[i].rfind("__HIGHLO_", 0) == 0)
					{
						specialMacros.emplace(tokens[i]);
					}
				}
			}
			else if (Lang == ShaderLanguage::GLSL)
			{
				if (tokens[index] == "version")
				{
					++index;
					startOfStage = pos;
				}
			}

			pos = newSource.find('#', pos + 1);
		}

		HL_ASSERT(stagePositions.size(), "Could not pre-process shader! There are no known shaders defined in file.");

		// Get first stage
		auto &[firstStage, firstStagePos] = stagePositions[0];
		if (stagePositions.size() > 1)
		{
			const std::string firstStageStr = newSource.substr(0, stagePositions[1].second);
			shaderSources[firstStage] = firstStageStr;
		}
		else
		{
			// we only have one shader source
			shaderSources[firstStage] = newSource;
		}

		for (uint64 i = 1; i < stagePositions.size(); ++i)
		{
			auto &[stage, stagePos] = stagePositions[i];

			if ((i + 1) >= stagePositions.size())
			{
				std::string lastStageStr = newSource.substr(stagePos);
				shaderSources[stage] = lastStageStr;
				break;
			}

			std::string stageStr = newSource.substr(stagePos, stagePositions[i + 1].second - stagePos);
			shaderSources[stage] = stageStr;
		}

		// TODO: Investigate why weird characters are before the #version
		// Cleanup: make sure nothing is before a #version command
		for (auto &[stage, source] : shaderSources)
		{
			uint32 versionPos = source.IndexOf("#version");
			if (versionPos > 0)
			{
				HLString newS = source.Substr(versionPos);
				shaderSources[stage] = newS;
			}
		}

		return shaderSources;
	}
}

