// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanShaderCompiler.h"

#include "Engine/Core/FileSystem.h"

#ifdef HIGHLO_API_VULKAN

#include <spirv_glsl.hpp>
#include <spirv-tools/libspirv.h>

#define SHADER_LOG_PREFIX "Shader>       "

namespace highlo
{
	struct VulkanCompileData
	{
		uint32 ShaderID = 0;
		uint32 VertexShaderID = 0;
		uint32 FragmentShaderID = 0;
		uint32 TessControlShaderID = 0;
		uint32 TessEvaluationShaderID = 0;
		uint32 GeometryShaderID = 0;
		uint32 ComputeShaderID = 0;
	};

	static VulkanCompileData s_VulkanCompiledData;

	VulkanShaderCompiler::VulkanShaderCompiler(const FileSystemPath &filePath, const BufferLayout &layout)
	{
		m_FilePath = filePath;
		m_Layout = layout;
		m_Source.FilePath = filePath;

		if (!FileSystem::Get()->FileExists(m_Source.FilePath))
		{
			HL_CORE_ERROR("{0} [-] Shader {1} does not exist! [-]", SHADER_LOG_PREFIX, *m_Source.FilePath.String());
			return;
		}

		s_VulkanCompiledData.ShaderID = 0;
		s_VulkanCompiledData.VertexShaderID = 0;
		s_VulkanCompiledData.FragmentShaderID = 0;
		s_VulkanCompiledData.TessControlShaderID = 0;
		s_VulkanCompiledData.TessEvaluationShaderID = 0;
		s_VulkanCompiledData.GeometryShaderID = 0;
		s_VulkanCompiledData.ComputeShaderID = 0;

		// Parse Shader
		std::unordered_map<ShaderType, std::stringstream> shaderSources;
		shaderSources[ShaderType::VERTEX_SHADER] = std::stringstream();
		shaderSources[ShaderType::TESS_CONTROL_SHADER] = std::stringstream();
		shaderSources[ShaderType::TESS_EVAL_SHADER] = std::stringstream();
		shaderSources[ShaderType::GEOMETRY_SHADER] = std::stringstream();
		shaderSources[ShaderType::PIXEL_SHADER] = std::stringstream();
		shaderSources[ShaderType::COMPUTE_SHADER] = std::stringstream();

		std::ifstream file;
		try
		{
			file.open(m_Source.FilePath.Absolute());

			std::string line;
			ShaderType type = ShaderType::NONE;

			while (std::getline(file, line))
			{
				if (line.find("#shader") != std::string::npos)
				{
					if (line.find("vertex") != std::string::npos)
						type = ShaderType::VERTEX_SHADER;
					else if (line.find("tess_control") != std::string::npos)
						type = ShaderType::TESS_CONTROL_SHADER;
					else if (line.find("tess_eval") != std::string::npos)
						type = ShaderType::TESS_EVAL_SHADER;
					else if (line.find("geometry") != std::string::npos)
						type = ShaderType::GEOMETRY_SHADER;
					else if (line.find("pixel") != std::string::npos)
						type = ShaderType::PIXEL_SHADER;
					else if (line.find("compute") != std::string::npos)
						type = ShaderType::COMPUTE_SHADER;
				}
				else
				{
					shaderSources[type] << line << "\n";
				}
			}
		}
		catch (std::ifstream::failure e)
		{
			HL_CORE_ERROR("{0} [-] Failed to load shader: {1} [-]", SHADER_LOG_PREFIX, *m_Source.FilePath.String());
			HL_CORE_ERROR(strerror(errno));
		}

		m_Source.VertexShaderSrc = HLString(shaderSources[ShaderType::VERTEX_SHADER].str().c_str());
		m_Source.TessellationControlShaderSrc = HLString(shaderSources[ShaderType::TESS_CONTROL_SHADER].str().c_str());
		m_Source.TessellationEvaluationShaderSrc = HLString(shaderSources[ShaderType::TESS_EVAL_SHADER].str().c_str());
		m_Source.GeometryShaderSrc = HLString(shaderSources[ShaderType::GEOMETRY_SHADER].str().c_str());
		m_Source.PixelShaderSrc = HLString(shaderSources[ShaderType::PIXEL_SHADER].str().c_str());
		m_Source.ComputeShaderSrc = HLString(shaderSources[ShaderType::COMPUTE_SHADER].str().c_str());
		m_State = ShaderCompilerState::Ready;
		HL_CORE_INFO("{0}[+] Loaded {1} [+]", SHADER_LOG_PREFIX, *m_Source.FilePath.String());
	}

	VulkanShaderCompiler::~VulkanShaderCompiler()
	{
		m_State = ShaderCompilerState::None;
	}

	void VulkanShaderCompiler::Compile()
	{
		m_State = ShaderCompilerState::Compiled;
	}

	void VulkanShaderCompiler::Link()
	{
		m_State = ShaderCompilerState::Linked;
	}
	
	uint32 VulkanShaderCompiler::CompileGLSLShader(const char *code, uint32 type)
	{
		uint32 shaderID = 0;

		return shaderID;
	}
}

#endif // HIGHLO_API_VULKAN

