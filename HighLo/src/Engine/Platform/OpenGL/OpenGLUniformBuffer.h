// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Renderer/Shaders/UniformBuffer.h"

#ifdef HIGHLO_API_OPENGL

namespace highlo
{
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:

		OpenGLUniformBuffer(uint32 size, uint32 binding);
		virtual ~OpenGLUniformBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void *data, uint32 size, uint32 offset = 0) override;
		virtual uint32 GetBinding() const override { return m_Binding; }

	private:

		HLRendererID m_RendererID = 0;
		uint32 m_Size = 0;
		uint32 m_Binding = 0;
		uint8 *m_LocalStorage = nullptr;
	};
}

#endif
