// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include <map>

#include "Engine/Renderer/BufferLayout.h"
#include "UniformBuffer.h"
#include "ShaderSource.h"
#include "Engine/Core/DataTypes/String.h"

namespace highlo
{
	class Shader : public IsSharedReference
	{
	public:

		HLAPI virtual void Bind() const = 0;
		HLAPI virtual void Unbind() = 0;
		HLAPI virtual HLRendererID GetRendererID() = 0;
		HLAPI virtual const HLString &GetName() const = 0;

		HLAPI Ref<UniformBuffer> GetBuffer(const HLString &name);
		HLAPI void AddBuffer(const HLString &name, Ref<UniformBuffer> buffer);

		HLAPI static Ref<UniformBuffer> &GetVSSceneUniformBuffer() { return m_VS_SceneBuffer; }

		HLAPI static Ref<Shader> Create(const ShaderSource &source, const BufferLayout &layout);

	private:

		std::map<HLString, Ref<UniformBuffer>> m_BufferMap;

		static void CreateVSSceneBuffer();
		static Ref<UniformBuffer> m_VS_SceneBuffer;
	};
}

