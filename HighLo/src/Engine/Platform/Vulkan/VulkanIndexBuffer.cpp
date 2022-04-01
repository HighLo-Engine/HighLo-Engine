#include "HighLoPch.h"
#include "VulkanIndexBuffer.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanIndexBuffer::VulkanIndexBuffer(std::vector<int32> &indices)
	{
	}

	VulkanIndexBuffer::VulkanIndexBuffer(void *data, uint32 size)
	{
	}
	
	VulkanIndexBuffer::VulkanIndexBuffer(uint32 size)
	{
	}
	
	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
	}
	
	void VulkanIndexBuffer::Bind() const
	{
	}
	
	void VulkanIndexBuffer::Unbind() const
	{
	}
	
	uint32 VulkanIndexBuffer::GetCount()
	{
		return uint32();
	}
	
	void VulkanIndexBuffer::UpdateContents(void *data, uint32 size, uint32 offset)
	{
	}
	
	void VulkanIndexBuffer::UpdateContents(std::vector<int32> &indices, uint32 offset)
	{
	}
}

#endif // HIGHLO_API_VULKAN

