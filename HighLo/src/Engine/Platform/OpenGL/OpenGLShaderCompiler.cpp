// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLShaderCompiler.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Core/FileSystem.h"

#ifdef HIGHLO_API_OPENGL

#define SHADER_LOG_PREFIX "Shader>       "

namespace highlo
{
	struct OpenGLCompileData
	{
		uint32 ShaderID = 0;
		uint32 VertexShaderID = 0;
		uint32 FragmentShaderID = 0;
		uint32 TessControlShaderID = 0;
		uint32 TessEvaluationShaderID = 0;
		uint32 GeometryShaderID = 0;
		uint32 ComputeShaderID = 0;
	};

	static OpenGLCompileData s_GLCompiledData;

	OpenGLShaderCompiler::OpenGLShaderCompiler(const FileSystemPath &filePath, const BufferLayout &layout)
	{
		m_FilePath = filePath;
		m_Layout = layout;
		m_Source.FilePath = filePath;

		if (!FileSystem::Get()->FileExists(m_Source.FilePath))
		{
			HL_CORE_ERROR("{0}[-] Shader {1} does not exist [-]!", SHADER_LOG_PREFIX, *m_Source.FilePath.String());
			return;
		}

		s_GLCompiledData.ShaderID = 0;
		s_GLCompiledData.VertexShaderID = 0;
		s_GLCompiledData.FragmentShaderID = 0;
		s_GLCompiledData.TessControlShaderID = 0;
		s_GLCompiledData.TessEvaluationShaderID = 0;
		s_GLCompiledData.GeometryShaderID = 0;
		s_GLCompiledData.ComputeShaderID = 0;

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
			HL_CORE_ERROR("{0}[-] Failed to load shader: {1} [-]", SHADER_LOG_PREFIX, *m_Source.FilePath.String());
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

	OpenGLShaderCompiler::~OpenGLShaderCompiler()
	{
		m_State = ShaderCompilerState::None;
	}

	void OpenGLShaderCompiler::Compile()
	{
		HL_CORE_INFO("{0}[+] Compiling Shader {1} [+]", SHADER_LOG_PREFIX, *m_Source.FilePath.String());

		s_GLCompiledData.ShaderID = glCreateProgram();

		if (!m_Source.VertexShaderSrc.IsEmpty())
		{
			s_GLCompiledData.VertexShaderID = CompileGLSLShader(*m_Source.VertexShaderSrc, GL_VERTEX_SHADER);
			glAttachShader(s_GLCompiledData.ShaderID, s_GLCompiledData.VertexShaderID);
		}

		if (!m_Source.PixelShaderSrc.IsEmpty())
		{
			s_GLCompiledData.ComputeShaderID = CompileGLSLShader(*m_Source.PixelShaderSrc, GL_FRAGMENT_SHADER);
			glAttachShader(s_GLCompiledData.ShaderID, s_GLCompiledData.ComputeShaderID);
		}

		if (!m_Source.TessellationControlShaderSrc.IsEmpty())
		{
			s_GLCompiledData.TessControlShaderID = CompileGLSLShader(*m_Source.TessellationControlShaderSrc, GL_TESS_CONTROL_SHADER);
			glAttachShader(s_GLCompiledData.ShaderID, s_GLCompiledData.TessControlShaderID);
		}

		if (!m_Source.TessellationEvaluationShaderSrc.IsEmpty())
		{
			s_GLCompiledData.TessEvaluationShaderID = CompileGLSLShader(*m_Source.TessellationEvaluationShaderSrc, GL_TESS_EVALUATION_SHADER);
			glAttachShader(s_GLCompiledData.ShaderID, s_GLCompiledData.TessEvaluationShaderID);
		}

		if (!m_Source.GeometryShaderSrc.IsEmpty())
		{
			s_GLCompiledData.GeometryShaderID = CompileGLSLShader(*m_Source.GeometryShaderSrc, GL_GEOMETRY_SHADER);
			glAttachShader(s_GLCompiledData.ShaderID, s_GLCompiledData.GeometryShaderID);
		}

		if (!m_Source.ComputeShaderSrc.IsEmpty())
		{
			s_GLCompiledData.ComputeShaderID = CompileGLSLShader(*m_Source.ComputeShaderSrc, GL_COMPUTE_SHADER);
			glAttachShader(s_GLCompiledData.ShaderID, s_GLCompiledData.ComputeShaderID);
		}

		HL_CORE_INFO("{0}[+] Compiling Shader successful {1} [+]", SHADER_LOG_PREFIX, *m_Source.FilePath.String());
		m_State = ShaderCompilerState::Compiled;
	}

	void OpenGLShaderCompiler::Link()
	{
		HL_CORE_INFO("{0}[+] Linking Shader {1} [+]", SHADER_LOG_PREFIX, *m_Source.FilePath.String());

		glLinkProgram(s_GLCompiledData.ShaderID);

		int32 isLinked = 0;
		glGetProgramiv(s_GLCompiledData.ShaderID, GL_LINK_STATUS, (int32*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(s_GLCompiledData.ShaderID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> info_log(maxLength);
			glGetProgramInfoLog(s_GLCompiledData.ShaderID, maxLength, &maxLength, &info_log[0]);

			glDeleteProgram(s_GLCompiledData.ShaderID);

			if (s_GLCompiledData.VertexShaderID)  glDeleteShader(s_GLCompiledData.VertexShaderID);
			if (s_GLCompiledData.FragmentShaderID)  glDeleteShader(s_GLCompiledData.FragmentShaderID);
			if (s_GLCompiledData.TessControlShaderID) glDeleteShader(s_GLCompiledData.TessControlShaderID);
			if (s_GLCompiledData.TessEvaluationShaderID) glDeleteShader(s_GLCompiledData.TessEvaluationShaderID);
			if (s_GLCompiledData.GeometryShaderID)  glDeleteShader(s_GLCompiledData.GeometryShaderID);
			if (s_GLCompiledData.ComputeShaderID)  glDeleteShader(s_GLCompiledData.ComputeShaderID);

			HL_CORE_ERROR(info_log.data());
			HL_CORE_ERROR("{0}[-] Failed to link shader {1} [-]", SHADER_LOG_PREFIX, *m_FilePath);
			return;
		}

		// Shader linked successfully, create shader instance

		HL_CORE_INFO("{0}[+] Linking Shader {1} successful [+]", SHADER_LOG_PREFIX, *m_Source.FilePath.String());
		m_Shader = Shader::Create(m_Source, m_Layout);

		// Cleanup
		if (s_GLCompiledData.VertexShaderID)  glDetachShader(s_GLCompiledData.ShaderID, s_GLCompiledData.VertexShaderID);
		if (s_GLCompiledData.FragmentShaderID)  glDetachShader(s_GLCompiledData.ShaderID, s_GLCompiledData.FragmentShaderID);
		if (s_GLCompiledData.TessControlShaderID) glDetachShader(s_GLCompiledData.ShaderID, s_GLCompiledData.TessControlShaderID);
		if (s_GLCompiledData.TessEvaluationShaderID) glDetachShader(s_GLCompiledData.ShaderID, s_GLCompiledData.TessEvaluationShaderID);
		if (s_GLCompiledData.GeometryShaderID)  glDetachShader(s_GLCompiledData.ShaderID, s_GLCompiledData.GeometryShaderID);
		if (s_GLCompiledData.ComputeShaderID)  glDetachShader(s_GLCompiledData.ShaderID, s_GLCompiledData.ComputeShaderID);

		m_State = ShaderCompilerState::Linked;
	}
	
	uint32 OpenGLShaderCompiler::CompileGLSLShader(const char *code, uint32 type)
	{
		uint32 shaderID = glCreateShader(type);

		glShaderSource(shaderID, 1, &code, 0);
		glCompileShader(shaderID);

		int32 isCompiled = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> info_log(maxLength);
			glGetShaderInfoLog(shaderID, maxLength, &maxLength, &info_log[0]);

			glDeleteShader(shaderID);

			HL_CORE_ERROR(info_log.data());
			HL_CORE_ERROR("{0}[-] Failed to compile shader {1} [-]", SHADER_LOG_PREFIX, *m_Source.FilePath.String());
			return 0;
		}

		return shaderID;
	}
}

#endif // HIGHLO_API_OPENGL

