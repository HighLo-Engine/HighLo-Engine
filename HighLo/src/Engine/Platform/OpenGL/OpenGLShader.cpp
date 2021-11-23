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
	{
		m_ID = source.ShaderID;
		m_Name = source.FilePath.Filename();
		if (m_Name.Contains("."))
			m_Name = m_Name.Substr(0, m_Name.IndexOf("."));
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
}

#endif
