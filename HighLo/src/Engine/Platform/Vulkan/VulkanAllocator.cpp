#include "HighLoPch.h"
#include "VulkanAllocator.h"

#include "VulkanContext.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
    struct VMAAllocationData
    {
        VmaAllocator Allocator;
        uint64 TotalAllocatedBytes = 0;
    };

    static VMAAllocationData *s_AllocationData = nullptr;

    VulkanAllocator::VulkanAllocator(const HLString &tag)
        : m_Tag(tag)
    {
    }
    
    VulkanAllocator::~VulkanAllocator()
    {
    }
    
    void VulkanAllocator::Init(Ref<VulkanDevice> &device)
    {
        s_AllocationData = new VMAAllocationData();

        VmaAllocatorCreateInfo allocatorInfo = {};
        allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_2;
        allocatorInfo.physicalDevice = device->GetPhysicalDevice()->GetNativePhysicalDevice();
        allocatorInfo.device = device->GetNativePhysicalDevice();
        allocatorInfo.instance = VulkanContext::GetInstance();

        vmaCreateAllocator(&allocatorInfo, &s_AllocationData->Allocator);
    }
    
    void VulkanAllocator::Shutdown()
    {
        vmaDestroyAllocator(s_AllocationData->Allocator);

        delete s_AllocationData;
        s_AllocationData = nullptr;
    }
    
    void VulkanAllocator::DumpStats()
    {
        // TODO
    }
    
    GPUMemoryStats VulkanAllocator::GetStats()
    {
        const auto &memoryProps = VulkanContext::GetCurrentDevice()->GetPhysicalDevice()->GetMemoryProperties();
        std::vector<VmaBudget> budgets(memoryProps.memoryHeapCount);
        uint64 usage = 0;
        uint64 budget = 0;

        for (VmaBudget &b : budgets)
        {
            usage += b.usage;
            budget += b.budget;
        }

        return { usage, budget - usage };
    }
    
    VmaAllocator &VulkanAllocator::GetVMAAllocator()
    {
        return s_AllocationData->Allocator;
    }
    
    VmaAllocation VulkanAllocator::AllocateBuffer(VkBufferCreateInfo bufferCreateInfo, VmaMemoryUsage usage, VkBuffer &outBuffer)
    {
        VmaAllocationCreateInfo allocCreateInfo = {};
        VmaAllocationInfo allocInfo{};
        VmaAllocation result;

        // Create Buffer
        allocCreateInfo.usage = usage;
        vmaCreateBuffer(s_AllocationData->Allocator, &bufferCreateInfo, &allocCreateInfo, &outBuffer, &result, nullptr);
        
        // Retrieve buffer infos
        vmaGetAllocationInfo(s_AllocationData->Allocator, result, &allocInfo);
        s_AllocationData->TotalAllocatedBytes += allocInfo.size;

        return result;
    }
    
    VmaAllocation VulkanAllocator::AllocateImage(VkImageCreateInfo imageCreateInfo, VmaMemoryUsage usage, VkImage &outImage)
    {
        VmaAllocationCreateInfo allocCreateInfo = {};
        VmaAllocationInfo allocInfo{};
        VmaAllocation result;

        // Create Image
        allocCreateInfo.usage = usage;
        vmaCreateImage(s_AllocationData->Allocator, &imageCreateInfo, &allocCreateInfo, &outImage, &result, nullptr);

        // Retrieve image infos
        vmaGetAllocationInfo(s_AllocationData->Allocator, result, &allocInfo);
        s_AllocationData->TotalAllocatedBytes += allocInfo.size;

        return result;
    }
    
    void VulkanAllocator::Free(VmaAllocation allocation)
    {
        vmaFreeMemory(s_AllocationData->Allocator, allocation);
    }
    
    void VulkanAllocator::DestroyImage(VkImage image, VmaAllocation allocation)
    {
        HL_ASSERT(image);
        HL_ASSERT(allocation);
        vmaDestroyImage(s_AllocationData->Allocator, image, allocation);
    }
    
    void VulkanAllocator::DestroyBuffer(VkBuffer buffer, VmaAllocation allocation)
    {
        HL_ASSERT(buffer);
        HL_ASSERT(allocation);
        vmaDestroyBuffer(s_AllocationData->Allocator, buffer, allocation);
    }
    
    void VulkanAllocator::UnmapMemory(VmaAllocation allocation)
    {
        vmaUnmapMemory(s_AllocationData->Allocator, allocation);
    }
}

#endif // HIGHLO_API_VULKAN

