// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#include "Engine/Renderer/ShaderCompiler.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

namespace highlo
{
	class OpenGLShaderCompiler : public ShaderCompiler
	{
	public:

		OpenGLShaderCompiler(const FileSystemPath &filePath, const BufferLayout &layout);
		virtual ~OpenGLShaderCompiler();

		virtual void Compile() override;
		virtual void Link() override;

		virtual const ShaderSource &GetShaderSource() const override { return m_Source; }
		virtual const BufferLayout &GetLayout() const override { return m_Layout; }
		virtual const FileSystemPath &GetFilePath() const override { return m_FilePath; }

	private:

		uint32 CompileGLSLShader(const char *code, uint32 type);

		ShaderSource m_Source;
		BufferLayout m_Layout;
		FileSystemPath m_FilePath;
	};
}

#endif // HIGHLO_API_OPENGL

