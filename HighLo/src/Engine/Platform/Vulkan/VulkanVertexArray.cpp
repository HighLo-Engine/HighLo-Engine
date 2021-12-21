// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanVertexArray.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanVertexArray::VulkanVertexArray()
	{
	}

	VulkanVertexArray::~VulkanVertexArray()
	{
	}

	void VulkanVertexArray::Bind() const
	{
	}

	void VulkanVertexArray::Unbind() const
	{
	}

	void VulkanVertexArray::Invalidate()
	{
	}

	void VulkanVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer)
	{
	}

	void VulkanVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer)
	{
	}
}

#endif // HIGHLO_API_VULKAN

