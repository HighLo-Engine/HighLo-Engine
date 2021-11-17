// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "OpenGLShader.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

#include "Engine/Core/FileSystem.h"
#include "Engine/Core/FileSystemPath.h"
#include "Engine/Renderer/Renderer.h"

namespace highlo
{
	OpenGLShader::OpenGLShader(const ShaderSource &source)
		: m_FileName(source.FileName)
	{
		m_Name = FileSystemPath(source.FileName).Filename();
		if (m_Name.Contains("."))
		{
			m_Name = m_Name.Substr(0, m_Name.IndexOf("."));
		}

		if (!FileSystem::Get()->FileExists(source.FileName))
		{
			HL_CORE_ERROR("Shader {0} does not exist!", *m_FileName);
			return;
		}

		CompileGLSLProgram(source);
	}

	OpenGLShader::~OpenGLShader()
	{
		GLuint rendererID = m_ID;
		Renderer::Submit([rendererID]()
		{
			glDeleteProgram(rendererID);
		});
	}

	void OpenGLShader::Bind() const
	{
		Ref<const OpenGLShader> instance = this;
		Renderer::Submit([instance]()
		{
			glUseProgram(instance->m_ID);
		});
	}

	void OpenGLShader::Unbind()
	{
		Renderer::Submit([]()
		{
			glUseProgram(0);
		});
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
		Ref<OpenGLShader> instance = this;
		Renderer::Submit([instance, source]() mutable
		{
			instance->m_ID = glCreateProgram();

			uint32 vs_id = 0;
			uint32 ps_id = 0;
			uint32 tcs_id = 0;
			uint32 tes_id = 0;
			uint32 gs_id = 0;
			uint32 cs_id = 0;

			if (!source.VertexShaderSrc.IsEmpty())
			{
				vs_id = instance->CompileGLSLShader(*source.VertexShaderSrc, GL_VERTEX_SHADER);
				glAttachShader(instance->m_ID, vs_id);
			}

			if (!source.PixelShaderSrc.IsEmpty())
			{
				ps_id = instance->CompileGLSLShader(*source.PixelShaderSrc, GL_FRAGMENT_SHADER);
				glAttachShader(instance->m_ID, ps_id);
			}

			if (!source.TessellationControlShaderSrc.IsEmpty())
			{
				tcs_id = instance->CompileGLSLShader(*source.TessellationControlShaderSrc, GL_TESS_CONTROL_SHADER);
				glAttachShader(instance->m_ID, tcs_id);
			}

			if (!source.TessellationEvaluationShaderSrc.IsEmpty())
			{
				tes_id = instance->CompileGLSLShader(*source.TessellationEvaluationShaderSrc, GL_TESS_EVALUATION_SHADER);
				glAttachShader(instance->m_ID, tes_id);
			}

			if (!source.GeometryShaderSrc.IsEmpty())
			{
				gs_id = instance->CompileGLSLShader(*source.GeometryShaderSrc, GL_GEOMETRY_SHADER);
				glAttachShader(instance->m_ID, gs_id);
			}

			if (!source.ComputeShaderSrc.IsEmpty())
			{
				cs_id = instance->CompileGLSLShader(*source.ComputeShaderSrc, GL_COMPUTE_SHADER);
				glAttachShader(instance->m_ID, cs_id);
			}

			glLinkProgram(instance->m_ID);

			int32 isLinked = 0;
			glGetProgramiv(instance->m_ID, GL_LINK_STATUS, (int32*)&isLinked);
			if (isLinked == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetProgramiv(instance->m_ID, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> info_log(maxLength);
				glGetProgramInfoLog(instance->m_ID, maxLength, &maxLength, &info_log[0]);

				glDeleteProgram(instance->m_ID);

				if (vs_id)  glDeleteShader(vs_id);
				if (ps_id)  glDeleteShader(ps_id);
				if (tcs_id) glDeleteShader(tcs_id);
				if (tes_id) glDeleteShader(tes_id);
				if (gs_id)  glDeleteShader(gs_id);
				if (cs_id)  glDeleteShader(cs_id);

				HL_CORE_ERROR(info_log.data());
				HL_CORE_ERROR("Failed to link shader {0}", *instance->m_FileName);
				return;
			}

			if (vs_id)  glDetachShader(instance->m_ID, vs_id);
			if (ps_id)  glDetachShader(instance->m_ID, ps_id);
			if (tcs_id) glDetachShader(instance->m_ID, tcs_id);
			if (tes_id) glDetachShader(instance->m_ID, tes_id);
			if (gs_id)  glDetachShader(instance->m_ID, gs_id);
			if (cs_id)  glDetachShader(instance->m_ID, cs_id);
		});
	}
}

#endif
