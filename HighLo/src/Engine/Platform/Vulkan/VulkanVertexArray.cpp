#include "HighLoPch.h"
#include "VulkanVertexArray.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
    VulkanVertexArray::VulkanVertexArray(const VertexArraySpecification &spec)
        : m_Specification(spec)
    {
        Invalidate();
    }
    
    VulkanVertexArray::~VulkanVertexArray()
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
