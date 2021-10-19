// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.2 (2021-10-16) fixed indentations and added verification check whether the shader file exists
//     - 1.1 (2021-09-15) Changed m_Name implementation
//     - 1.0 (2021-09-14) initial release
//

#pragma once
#include "Engine/Renderer/Shader.h"
#include "Engine/Core/Defines/BaseTypes.h"

#ifdef HIGHLO_API_OPENGL

namespace highlo
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const ShaderSource &source);
		~OpenGLShader();

		// Inherited via Shader
		virtual void Bind() const override;
		virtual void Unbind() override;
		virtual HLRendererID GetRendererID() override { return m_ID; }
		virtual const HLString &GetName() const override { return m_Name; }

	private:

		uint32 CompileGLSLShader(const char *code, uint32 type);
		void CompileGLSLProgram(const ShaderSource &source);

		HLString m_FileName;
		HLString m_Name;
		HLRendererID m_ID = 0;
	};
}

#endif
