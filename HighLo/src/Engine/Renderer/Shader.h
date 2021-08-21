#pragma once
#include "BufferLayout.h"
#include "UniformBuffer.h"
#include "Engine/Core/DataTypes/String.h"
#include <map>

namespace highlo
{
	struct ShaderSource
	{
		HLString m_FileName;
		HLString m_VertexShaderSrc;
		HLString m_TessellationControlShaderSrc;
		HLString m_TessellationEvaluationShaderSrc;
		HLString m_GeometryShaderSrc;
		HLString m_PixelShaderSrc;
		HLString m_ComputeShaderSrc;
	};

	class Shader : public IsSharedReference
	{
	public:
		static ShaderSource LoadShaderSource(const HLString& filename);

		static Ref<Shader> Create(const ShaderSource& source, const BufferLayout& layout);
		static Ref<Shader> CreateComputeShader(const ShaderSource& source);

		virtual void Bind() const = 0;
		virtual HLRendererID GetRendererID() = 0;
		virtual const HLString &GetName() const = 0;

		Ref<UniformBuffer> GetBuffer(const HLString& name);
		void AddBuffer(const HLString& name, Ref<UniformBuffer> buffer);

		static Ref<UniformBuffer>& GetVSSceneUniformBuffer() { return m_VS_SceneBuffer; }

	private:
		std::map<HLString, Ref<UniformBuffer>> m_BufferMap;

	private:
		static void CreateVSSceneBuffer();
		static Ref<UniformBuffer> m_VS_SceneBuffer;
	};
}
