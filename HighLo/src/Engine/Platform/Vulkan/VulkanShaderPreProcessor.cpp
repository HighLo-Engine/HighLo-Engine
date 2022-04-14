#include "HighLoPch.h"
#include "VulkanShaderPreProcessor.h"

#ifdef HIGHLO_API_VULKAN

#include "VulkanUtils.h"

#include <regex>

namespace highlo
{
	namespace utils
	{
		static std::vector<std::string> SplitStringsAndKeepDelimiters(std::string &str)
		{
			const static std::regex re(R"((^\W|^\w+)|(\w+)|[:()])", std::regex_constants::optimize);

			std::regex_iterator<std::string::iterator> rit(str.begin(), str.end(), re);
			std::regex_iterator<std::string::iterator> rend;
			std::vector<std::string> result;

			while (rit != rend)
			{
				result.emplace_back(rit->str());
				++rit;
			}

			return result;
		}

		static bool ContainsHeaderGuard(std::string &header)
		{
			uint64 pos = header.find('#');
			while (pos != std::string::npos)
			{
				const uint64 endOfLine = header.find_first_of("\r\n", pos) + 1;
				auto &tokens = utils::SplitStringsAndKeepDelimiters(header.substr(pos, endOfLine - pos));
				auto it = tokens.begin();

				if (*(++it) == "pragma")
				{
					if (*(++it) == "once")
					{
						return true;
					}
				}

				pos = header.find('#', pos + 1);
			}

			return false;
		}

		enum class State : char
		{
			SlashOC,
			StarIC,
			SingleLineComment,
			MultiLineComment,
			NotAComment
		};

		template<typename InputIt, typename OutputIt>
		static void CopyWithoutComments(InputIt first, InputIt last, OutputIt out)
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
	}

	ShaderType VulkanShaderPreProcessor::PreProcessHeader(HLString &contents, bool &isGuarded, std::unordered_set<HLString> &specialMacros, const std::unordered_set<IncludeData> &includeData, const FileSystemPath &fullPath)
	{
		std::stringstream sourceStream;
		std::string contentStdStr = *contents;
		utils::CopyWithoutComments(contentStdStr.begin(), contentStdStr.end(), std::ostream_iterator<char>(sourceStream));
		contentStdStr = sourceStream.str();

		VkShaderStageFlagBits stagesInHeader = {};

		isGuarded = utils::ContainsHeaderGuard(contentStdStr);

		uint32 stageCount = 0;
		uint64 startOfShaderStage = contentStdStr.find('#', 0);

		while (startOfShaderStage != std::string::npos)
		{
			const uint64 endOfLine = contentStdStr.find_first_of("\r\n", startOfShaderStage) + 1;
			auto &tokens = utils::SplitStringsAndKeepDelimiters(contentStdStr.substr(startOfShaderStage, endOfLine - startOfShaderStage));

			uint32 index = 0;
			if (tokens[index] == "#")
			{
				++index;

				if (index >= tokens.size())
				{
					HL_CORE_WARN("Could not find any pre-processor directive!");
					break;
				}

				if (tokens[index] == "shader")
				{
					HL_ASSERT(tokens[index + 1] == ":");

					const std::string_view stage(tokens[++index]);
					HL_ASSERT(stage == "vertex" || stage == "fragment" || stage == "pixel" || stage == "geometry" || stage == "compute" || stage == "tess_control" || stage == "tess_eval", "Invalid shader type");
					VkShaderStageFlagBits foundStage = utils::ShaderStageFromString(std::string(stage.begin(), stage.end()));

					const bool alreadyIncluded = std::find_if(includeData.begin(), includeData.end(), [fullPath](const IncludeData &data)
					{
						return fullPath == data.IncludedFilePath;
					}) != includeData.end();

					if (isGuarded && alreadyIncluded)
					{
						contentStdStr.clear();
					}
					else if (!isGuarded && alreadyIncluded)
					{
						HL_CORE_WARN("\"{0}\" Header does not contain a header guard (#pragma once).", **fullPath);
					}

					if (stageCount == 0)
					{
						contentStdStr.replace(startOfShaderStage, endOfLine - startOfShaderStage, fmt::format("#ifdef {}", utils::StageToShaderMacro(stage)));
					}
					else
					{
						contentStdStr.replace(startOfShaderStage, endOfLine - startOfShaderStage, fmt::format("#endif\r#ifdef {}", utils::StageToShaderMacro(stage)));
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

			startOfShaderStage = contentStdStr.find('#', startOfShaderStage + 1);
		}

		if (stageCount)
		{
			contentStdStr.append("\n#endif");
		}
		else
		{
			const bool alreadyIncluded = std::find_if(includeData.begin(), includeData.end(), [fullPath](const IncludeData &data)
			{
				return data.IncludedFilePath == fullPath;
			}) != includeData.end();

			if (isGuarded && alreadyIncluded)
			{
				contentStdStr.clear();
			}
			else if (!isGuarded && alreadyIncluded)
			{
				HL_CORE_WARN("{0} Header does not contain a header guard (#pragma once)", **fullPath);
			}
		}

		contents = contentStdStr;
		return utils::VulkanStageToShaderType(stagesInHeader);
	}

	std::unordered_map<ShaderType, HLString> VulkanShaderPreProcessor::PreProcessShader(const HLString &source, std::unordered_set<HLString> &specialMacros, ShaderLanguage language)
	{
		std::stringstream sourceStream;
		std::string contentStdStr = *source;
		utils::CopyWithoutComments(contentStdStr.begin(), contentStdStr.end(), std::ostream_iterator<char>(sourceStream));
		std::string newSource = sourceStream.str();

		std::unordered_map<VkShaderStageFlagBits, HLString> shaderSources;
		std::vector<std::pair<VkShaderStageFlagBits, uint64>> stagePositions;
		HL_ASSERT(newSource.size(), "Shader is empty!");

		uint64 startOfStage = 0;
		uint64 pos = newSource.find('#');

		// Check first #version
		if (language == ShaderLanguage::GLSL)
		{
			const uint64 endOfLine = newSource.find_first_of("\r\n", pos) + 1;
			const std::vector<std::string> &tokens = utils::SplitStringsAndKeepDelimiters(newSource.substr(pos, endOfLine - pos));
			HL_ASSERT(tokens.size() >= 3 && tokens[1] == "version", "Invalid #version encountered or #version is NOT encounted first.");
			pos = newSource.find('#', pos + 1);
		}

		while (pos != std::string::npos)
		{
			const uint64 endOfLine = newSource.find_first_of("\r\n", pos) + 1;
			std::vector<std::string> &tokens = utils::SplitStringsAndKeepDelimiters(newSource.substr(pos, endOfLine - pos));

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
					auto shaderStage = utils::ShaderStageFromString(std::string(stage.begin(), stage.end()));

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
			else if (language == ShaderLanguage::GLSL)
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

		return utils::ConvertVulkanStageToShaderType(shaderSources);
	}
}

#endif // HIGHLO_API_VULKAN

