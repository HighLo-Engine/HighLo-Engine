// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-11-19) initial release
//

#pragma once

#include "Engine/Graphics/VertexBuffer.h"

#include "Vulkan.h"

namespace highlo
{
	class VulkanVertexBuffer : public VertexBuffer
	{
	public:

		VulkanVertexBuffer(void *data, uint32 size, VertexBufferUsage usage = VertexBufferUsage::Static);
		VulkanVertexBuffer(uint32 size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);
		virtual ~VulkanVertexBuffer();

		virtual void Bind() const override {}
		virtual void Unbind() const override {}

		virtual void UpdateContents(void *data, uint32 size, uint32 offset = 0) override;

		virtual HLRendererID GetRendererID() override { return m_RendererID; }
		virtual VertexBufferUsage GetUsage() override { return m_Usage; }

	private:

		VertexBufferUsage m_Usage = VertexBufferUsage::None;
		HLRendererID m_RendererID = 0;
	};
}

