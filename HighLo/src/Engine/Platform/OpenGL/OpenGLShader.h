#pragma once
#include "Engine/Renderer/Shader.h"
#include "Engine/Core/Defines/HLBaseTypes.h"

#ifdef HIGHLO_API_OPENGL

namespace highlo
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const ShaderSource& source, bool isCompute = false);
		~OpenGLShader();

		// Inherited via Shader
		virtual void Bind() const override;
		virtual HLRendererID GetRendererID() override;
		virtual const HLString &GetName() const override { return m_Name; }

	private:
		uint32 CompileGLSLShader(const char* code, uint32 type);
		void CompileGLSLProgram(const ShaderSource& source);

		uint32 m_ID = 0;

	private:
		void CompileComputeShader(const ShaderSource& source);

		HLString m_FileName;
		HLString m_Name;
	};
}

#endif
