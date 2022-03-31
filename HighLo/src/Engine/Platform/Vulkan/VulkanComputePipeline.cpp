#include "HighLoPch.h"
#include "VulkanComputePipeline.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
    VulkanComputePipeline::VulkanComputePipeline(Ref<Shader> computeShader)
        : m_Shader(computeShader)
    {
        Invalidate();
    }

    VulkanComputePipeline::~VulkanComputePipeline()
    {
    }
    
    void VulkanComputePipeline::Begin(Ref<CommandBuffer> renderCommandBuffer)
    {
    }
    
    void VulkanComputePipeline::End()
    {
    }
    
    void VulkanComputePipeline::Invalidate()
    {
    }
}

#endif // HIGHLO_API_VULKAN

