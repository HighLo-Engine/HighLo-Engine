// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanImGuiRenderer.h"

#include "VulkanUtils.h"
#include "VulkanContext.h"
#include "VulkanSwapChain.h"

#ifdef HIGHLO_API_GLFW
#include <GLFW/glfw3.h>
#include "Engine/ImGui/ImGui/backends/imgui_impl_glfw.h"
#endif // HIGHLO_API_GLFW

#include "Engine/ImGui/ImGui/imgui.h"
#include "Engine/ImGui/ImGui/imgui_internal.h"
#include "Engine/ImGui/ImGui/backends/imgui_impl_vulkan_with_textures.h"

#include "Engine/Renderer/Renderer.h"

namespace highlo
{
    static std::vector<VkCommandBuffer> s_ImGuiCommandBuffers;

    VulkanImGuiRenderer::VulkanImGuiRenderer()
    {
    }

    VulkanImGuiRenderer::~VulkanImGuiRenderer()
    {
    }

    void VulkanImGuiRenderer::Init(Window *window)
    {
#if 0
        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        Ref<VulkanSwapChain> swapChain = HLApplication::Get().GetWindow().GetSwapChain().As<VulkanSwapChain>();
        VkDescriptorPoolSize poolSizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 100 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 100 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 100 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 100 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 100 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 100 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 100 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 100 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 100 }
        };

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 100 * (uint32)IM_ARRAYSIZE(poolSizes);
        poolInfo.poolSizeCount = (uint32)IM_ARRAYSIZE(poolSizes);
        poolInfo.pPoolSizes = poolSizes;

        VkDescriptorPool descriptorPool;
        VK_CHECK_RESULT(vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool));

#ifdef HIGHLO_API_GLFW
        ImGui_ImplGlfw_InitForVulkan((GLFWwindow *)window->GetNativeHandle(), true);
#else
        ImGui_ImplWin32_Init(window->GetNativeHandle(), window->GetNativeContext());
#endif // HIGHLO_API_GLFW

        ImGui_ImplVulkan_InitInfo initInfo = {};
        initInfo.Instance = VulkanContext::GetInstance();
        initInfo.PhysicalDevice = VulkanContext::GetCurrentDevice()->GetPhysicalDevice()->GetNativeDevice();
        initInfo.Device = device;
        initInfo.QueueFamily = VulkanContext::GetCurrentDevice()->GetPhysicalDevice()->GetQueueFamilyIndices().Graphics;
        initInfo.Queue = VulkanContext::GetCurrentDevice()->GetGraphicsQueue();
        initInfo.PipelineCache = nullptr;
        initInfo.DescriptorPool = descriptorPool;
        initInfo.Allocator = nullptr;
        initInfo.MinImageCount = 2;
        initInfo.ImageCount = swapChain->GetImageCount();
        initInfo.CheckVkResultFn = utils::VulkanCheckResult;

        ImGui_ImplVulkan_Init(&initInfo, swapChain->GetRenderPass());

        // Upload fonts
        VkCommandBuffer commandBuffer = VulkanContext::GetCurrentDevice()->CreateCommandBuffer(true);
        ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
        VulkanContext::GetCurrentDevice()->FlushCommandBuffer(commandBuffer);

        VK_CHECK_RESULT(vkDeviceWaitIdle(device));
        ImGui_ImplVulkan_DestroyFontUploadObjects();

        uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;
        s_ImGuiCommandBuffers.resize(framesInFlight);
        for (uint32 i = 0; i < framesInFlight; ++i)
            s_ImGuiCommandBuffers[i] = VulkanContext::GetCurrentDevice()->CreateSecondaryCommandBuffer();
#endif
    }

    void VulkanImGuiRenderer::Shutdown()
    {
#if 0
        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        VK_CHECK_RESULT(vkDeviceWaitIdle(device));
#endif
        ImGui_ImplVulkan_Shutdown();
    }

    void VulkanImGuiRenderer::NewFrame()
    {
        ImGui_ImplVulkan_NewFrame();
    }

    void VulkanImGuiRenderer::Render()
    {
        ImGui::Render();
    }

    void VulkanImGuiRenderer::RenderDrawData()
    {
#if 0
        VkDevice device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
        Ref<VulkanSwapChain> swapChain = HLApplication::Get().GetWindow().GetSwapChain().As<VulkanSwapChain>();
        uint32 width = swapChain->GetWidth();
        uint32 height = swapChain->GetHeight();
        uint32 commandBufferIndex = swapChain->GetCurrentBufferIndex();

        VkClearValue clearValues[2];
        clearValues[0].color = { { 0.1f, 0.1f, 0.1f, 1.0f } };
        clearValues[1].depthStencil = { 1.0f, 0 };

        VkCommandBufferBeginInfo drawCmdBufferInfo = {};
        drawCmdBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        drawCmdBufferInfo.pNext = nullptr;
        drawCmdBufferInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VkCommandBuffer drawCommandBuffer = swapChain->GetCurrentCommandbuffer();
        VK_CHECK_RESULT(vkBeginCommandBuffer(drawCommandBuffer, &drawCmdBufferInfo));

        VkRenderPassBeginInfo renderPassBeginInfo = {};
        renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.pNext = nullptr;
        renderPassBeginInfo.renderPass = swapChain->GetRenderPass();
        renderPassBeginInfo.framebuffer = swapChain->GetCurrentFramebuffer();
        renderPassBeginInfo.renderArea.offset.x = 0;
        renderPassBeginInfo.renderArea.offset.y = 0;
        renderPassBeginInfo.renderArea.extent.width = width;
        renderPassBeginInfo.renderArea.extent.height = height;
        renderPassBeginInfo.clearValueCount = 2;
        renderPassBeginInfo.pClearValues = clearValues;
        vkCmdBeginRenderPass(drawCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);

        VkCommandBufferInheritanceInfo inheritanceInfo = {};
        inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
        inheritanceInfo.pNext = nullptr;
        inheritanceInfo.renderPass = swapChain->GetRenderPass();
        inheritanceInfo.framebuffer = swapChain->GetCurrentFramebuffer();

        VkCommandBufferBeginInfo cmdBufferBeginInfo = {};
        cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        cmdBufferBeginInfo.pNext = nullptr;
        cmdBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
        cmdBufferBeginInfo.pInheritanceInfo = &inheritanceInfo;
        VK_CHECK_RESULT(vkBeginCommandBuffer(s_ImGuiCommandBuffers[commandBufferIndex], &cmdBufferBeginInfo));

        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = (float)height;
        viewport.width = (float)width;
        viewport.height = -(float)height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(s_ImGuiCommandBuffers[commandBufferIndex], 0, 1, &viewport);

        VkRect2D scissor = {};
        scissor.extent.width = width;
        scissor.extent.height = height;
        scissor.offset.x = 0;
        scissor.offset.y = 0;
        vkCmdSetScissor(s_ImGuiCommandBuffers[commandBufferIndex], 0, 1, &scissor);

        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), s_ImGuiCommandBuffers[commandBufferIndex]);
        VK_CHECK_RESULT(vkEndCommandBuffer(s_ImGuiCommandBuffers[commandBufferIndex]));

        std::vector<VkCommandBuffer> commandBuffers;
        commandBuffers.push_back(s_ImGuiCommandBuffers[commandBufferIndex]);
        vkCmdExecuteCommands(drawCommandBuffer, (uint32)commandBuffers.size(), commandBuffers.data());

        vkCmdEndRenderPass(drawCommandBuffer);
        VK_CHECK_RESULT(vkEndCommandBuffer(drawCommandBuffer));
#endif
    }
}

