// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLShader.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

#include "Engine/Core/File.h"
#include "Engine/Core/FileSystem.h"

namespace highlo
{
	OpenGLShader::OpenGLShader(const ShaderSource &source, bool isCompute)
		: m_FileName(source.FileName)
	{
		m_Name = File::GetFileName(source.FileName);

		if (!FileSystem::FileExists(source.FileName))
		{
			HL_CORE_ERROR("Shader {0} does not exist!", *m_FileName);
			return;
		}

		if (!isCompute)
			CompileGLSLProgram(source);
		else
			CompileComputeShader(source);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_ID);
	}

	HLRendererID OpenGLShader::GetRendererID()
	{
		return m_ID;
	}

	uint32 OpenGLShader::CompileGLSLShader(const char *code, uint32 type)
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
			HL_CORE_ERROR("Failed to compile shader {0}", *m_FileName);
			return 0;
		}

		return shaderID;
	}

	void OpenGLShader::CompileGLSLProgram(const ShaderSource &source)
	{
		m_ID = glCreateProgram();

		auto vs_id = CompileGLSLShader(source.VertexShaderSrc, GL_VERTEX_SHADER);
		auto ps_id = CompileGLSLShader(source.PixelShaderSrc, GL_FRAGMENT_SHADER);

		uint32 tcs_id = 0;
		uint32 tes_id = 0;
		uint32 gs_id = 0;

		glAttachShader(m_ID, vs_id);
		glAttachShader(m_ID, ps_id);

		if (!source.TessellationControlShaderSrc.IsEmpty())
		{
			tcs_id = CompileGLSLShader(source.TessellationControlShaderSrc, GL_TESS_CONTROL_SHADER);
			glAttachShader(m_ID, tcs_id);
		}

		if (!source.TessellationEvaluationShaderSrc.IsEmpty())
		{
			tes_id = CompileGLSLShader(source.TessellationEvaluationShaderSrc, GL_TESS_EVALUATION_SHADER);
			glAttachShader(m_ID, tes_id);
		}

		if (!source.GeometryShaderSrc.IsEmpty())
		{
			gs_id = CompileGLSLShader(source.GeometryShaderSrc, GL_GEOMETRY_SHADER);
			glAttachShader(m_ID, gs_id);
		}

		glLinkProgram(m_ID);

		int32 isLinked = 0;
		glGetProgramiv(m_ID, GL_LINK_STATUS, (int32*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> info_log(maxLength);
			glGetProgramInfoLog(m_ID, maxLength, &maxLength, &info_log[0]);

			glDeleteProgram(m_ID);
			glDeleteShader(vs_id);
			glDeleteShader(ps_id);

			if (tcs_id) glDeleteShader(tcs_id);
			if (tes_id) glDeleteShader(tes_id);
			if (gs_id)  glDeleteShader(gs_id);

			HL_CORE_ERROR(info_log.data());
			HL_CORE_ERROR("Failed to link shader {0}", *m_FileName);
			return;
		}

		glDetachShader(m_ID, vs_id);
		glDetachShader(m_ID, ps_id);

		if (tcs_id) glDetachShader(m_ID, tcs_id);
		if (tes_id) glDetachShader(m_ID, tes_id);
		if (gs_id)  glDetachShader(m_ID, gs_id);
	}

	void OpenGLShader::CompileComputeShader(const ShaderSource& source)
	{
		m_ID = glCreateProgram();

		auto cs_id = CompileGLSLShader(source.ComputeShaderSrc, GL_COMPUTE_SHADER);

		glAttachShader(m_ID, cs_id);
		glLinkProgram(m_ID);

		int32 isLinked = 0;
		glGetProgramiv(m_ID, GL_LINK_STATUS, (int32*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> info_log(maxLength);
			glGetProgramInfoLog(m_ID, maxLength, &maxLength, &info_log[0]);

			glDeleteProgram(m_ID);
			glDeleteShader(cs_id);

			HL_CORE_ERROR(info_log.data());
			HL_CORE_ERROR("Failed to link compute shader");
			return;
		}

		glDetachShader(m_ID, cs_id);
	}
}

#endif
