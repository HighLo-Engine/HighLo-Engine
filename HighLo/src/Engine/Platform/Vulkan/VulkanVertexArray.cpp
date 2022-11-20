// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanVertexArray.h"

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
        m_VertexBuffers.push_back(vertexBuffer);
    }
    
    void VulkanVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer)
    {
        m_IndexBuffer = indexBuffer;
    }
}

