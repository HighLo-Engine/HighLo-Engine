// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include <map>

#include "BufferLayout.h"
#include "UniformBuffer.h"
#include "Engine/Core/DataTypes/String.h"

namespace highlo
{
	struct ShaderSource
	{
		HLString FileName;
		HLString VertexShaderSrc;
		HLString TessellationControlShaderSrc;
		HLString TessellationEvaluationShaderSrc;
		HLString GeometryShaderSrc;
		HLString PixelShaderSrc;
		HLString ComputeShaderSrc;
	};

	class Shader : public IsSharedReference
	{
	public:
		static ShaderSource LoadShaderSource(const HLString &filename);

		static Ref<Shader> Create(const ShaderSource &source, const BufferLayout &layout);
		static Ref<Shader> CreateComputeShader(const ShaderSource &source);

		virtual void Bind() const = 0;
		virtual void Unbind() = 0;
		virtual HLRendererID GetRendererID() = 0;
		virtual const HLString &GetName() const = 0;

		Ref<UniformBuffer> GetBuffer(const HLString &name);
		void AddBuffer(const HLString &name, Ref<UniformBuffer> buffer);

		static Ref<UniformBuffer> &GetVSSceneUniformBuffer() { return m_VS_SceneBuffer; }

	private:
		std::map<HLString, Ref<UniformBuffer>> m_BufferMap;

		static void CreateVSSceneBuffer();
		static Ref<UniformBuffer> m_VS_SceneBuffer;
	};
}
