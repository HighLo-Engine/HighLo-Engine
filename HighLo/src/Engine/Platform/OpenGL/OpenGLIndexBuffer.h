// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Graphics/IndexBuffer.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/Allocator.h"

#ifdef HIGHLO_API_OPENGL
namespace highlo
{
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(std::vector<int32> &indices);
		OpenGLIndexBuffer(void *data, uint32 size);
		OpenGLIndexBuffer(uint32 size);
		~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void UpdateContents(void *data, uint32 size, uint32 offset = 0) override;
		virtual void UpdateContents(std::vector<int32> &indices, uint32 offset = 0) override;

		inline virtual uint32 GetCount() override { return m_Count; }
		inline virtual HLRendererID GetRendererID() override { return m_ID; }

	private:

		Allocator m_LocalData;
		HLRendererID m_ID = 0;
		uint32 m_Count = 0;
		uint32 m_Size = 0;
	};
}

#endif // HIGHLO_API_OPENGL
