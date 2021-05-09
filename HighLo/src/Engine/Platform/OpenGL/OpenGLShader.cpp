#include "HighLoPch.h"
#include "OpenGLShader.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

#include "Engine/Core/HLVirtualFileSystem.h"

namespace highlo
{
	OpenGLShader::OpenGLShader(const ShaderSource& source, bool isCompute)
		: m_FileName(source.m_FileName)
	{
		if (!isCompute)
			CompileGLSLProgram(source);
		else
			CompileComputeShader(source);

		m_Name = VirtualFileSystem::Get()->GetFileNameFromPath(source.m_FileName);
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

	uint32 OpenGLShader::CompileGLSLShader(const char* code, uint32 type)
	{
		uint32 shader_id = glCreateShader(type);

		glShaderSource(shader_id, 1, &code, 0);
		glCompileShader(shader_id);

		int is_compiled = 0;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &is_compiled);
		if (is_compiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> info_log(maxLength);
			glGetShaderInfoLog(shader_id, maxLength, &maxLength, &info_log[0]);

			glDeleteShader(shader_id);

			HL_CORE_ERROR(info_log.data());
			HL_CORE_ERROR("Failed to compile shader {0}", *m_FileName);
			return 0;
		}

		return shader_id;
	}

	void OpenGLShader::CompileGLSLProgram(const ShaderSource& source)
	{
		m_ID = glCreateProgram();

		auto vs_id = CompileGLSLShader(source.m_VertexShaderSrc, GL_VERTEX_SHADER);
		auto ps_id = CompileGLSLShader(source.m_PixelShaderSrc, GL_FRAGMENT_SHADER);

		uint32 tcs_id = 0;
		uint32 tes_id = 0;
		uint32 gs_id = 0;

		glAttachShader(m_ID, vs_id);
		glAttachShader(m_ID, ps_id);

		if (!source.m_TessellationControlShaderSrc.IsEmpty())
		{
			tcs_id = CompileGLSLShader(source.m_TessellationControlShaderSrc, GL_TESS_CONTROL_SHADER);
			glAttachShader(m_ID, tcs_id);
		}

		if (!source.m_TessellationEvaluationShaderSrc.IsEmpty())
		{
			tes_id = CompileGLSLShader(source.m_TessellationEvaluationShaderSrc, GL_TESS_EVALUATION_SHADER);
			glAttachShader(m_ID, tes_id);
		}

		if (!source.m_GeometryShaderSrc.IsEmpty())
		{
			gs_id = CompileGLSLShader(source.m_GeometryShaderSrc, GL_GEOMETRY_SHADER);
			glAttachShader(m_ID, gs_id);
		}

		glLinkProgram(m_ID);

		int is_linked = 0;
		glGetProgramiv(m_ID, GL_LINK_STATUS, (int*)&is_linked);
		if (is_linked == GL_FALSE)
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

		auto cs_id = CompileGLSLShader(source.m_ComputeShaderSrc, GL_COMPUTE_SHADER);

		glAttachShader(m_ID, cs_id);
		glLinkProgram(m_ID);

		int is_linked = 0;
		glGetProgramiv(m_ID, GL_LINK_STATUS, (int*)&is_linked);
		if (is_linked == GL_FALSE)
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
