#pragma once
#include "BufferLayout.h"
#include "UniformBuffer.h"
#include "Engine/Core/DataTypes/HLString.h"
#include <map>

namespace highlo
{
	struct ShaderSource
	{
		HLString m_VertexShaderSrc;
		HLString m_TessellationControlShaderSrc;
		HLString m_TessellationEvaluationShaderSrc;
		HLString m_GeometryShaderSrc;
		HLString m_PixelShaderSrc;
	};

	class Shader : public IsSharedReference
	{
		static const HLString GetDefaultEngineShaderFolder();

	public:
		static ShaderSource LoadShaderSource(const HLString& filename);
		static ShaderSource GetDefaultEngineStaticShaderSource();
		static ShaderSource GetDefaultEngineAnimatedShaderSource();

		static Ref<Shader> Create(const ShaderSource& source, const BufferLayout& layout);

		virtual void Bind() const = 0;

		Ref<UniformBuffer> GetBuffer(const HLString& name);
		void AddBuffer(const HLString& name, Ref<UniformBuffer> buffer);

	private:
		std::map<HLString, Ref<UniformBuffer>> m_BufferMap;
	};
}
