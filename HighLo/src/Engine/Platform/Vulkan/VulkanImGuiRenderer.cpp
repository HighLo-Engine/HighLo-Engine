// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanImGuiRenderer.h"

#ifdef HIGHLO_API_VULKAN

#ifdef HIGHLO_API_GLFW
#include <GLFW/glfw3.h>
#include "Engine/ImGui/ImGui/backends/imgui_impl_glfw.h"
#endif // HIGHLO_API_GLFW

#include "Engine/ImGui/ImGui/imgui.h"
#include "Engine/ImGui/ImGui/imgui_internal.h"
#include "Engine/ImGui/ImGui/backends/imgui_impl_vulkan_with_textures.h"

#include "VulkanContext.h"

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
		GLFWwindow *nativeWindow = static_cast<GLFWwindow*>(window->GetNativeHandle());
		Ref<VulkanSwapChain> swapChain = HLApplication::Get().GetWindow().GetSwapChain();

		auto vulkanContext = VulkanContext::Get();
		auto device = VulkanContext::GetCurrentDevice()->GetNativeDevice();

		VkDescriptorPool descriptorPool;

		// Create Descriptor Pool
		VkDescriptorPoolSize pool_sizes[] =
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
		poolInfo.maxSets = 100 * IM_ARRAYSIZE(pool_sizes);
		poolInfo.poolSizeCount = (uint32)IM_ARRAYSIZE(pool_sizes);
		poolInfo.pPoolSizes = pool_sizes;
		VK_CHECK_RESULT(vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool));

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForVulkan(nativeWindow, true);
		ImGui_ImplVulkan_InitInfo initInfo = {};
		initInfo.Instance = VulkanContext::GetInstance();
		initInfo.PhysicalDevice = VulkanContext::GetCurrentDevice()->GetPhysicalDevice()->GetNativePhysicalDevice();
		initInfo.Device = device;
		initInfo.QueueFamily = VulkanContext::GetCurrentDevice()->GetPhysicalDevice()->GetQueueFamilyIndices().Graphics;
		initInfo.Queue = VulkanContext::GetCurrentDevice()->GetGraphicsQueue();
		initInfo.PipelineCache = nullptr;
		initInfo.DescriptorPool = descriptorPool;
		initInfo.Allocator = nullptr;
		initInfo.MinImageCount = 2;
		initInfo.ImageCount = swapChain->GetImageCount();
		initInfo.CheckVkResultFn = utils::CheckResult;
		ImGui_ImplVulkan_Init(&initInfo, swapChain->GetNativeRenderPass());

		// Upload Fonts
		{
			// Use any command queue

			VkCommandBuffer commandBuffer = vulkanContext->GetCurrentDevice()->CreateCommandBuffer(true);
			ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
			vulkanContext->GetCurrentDevice()->FlushCommandBuffer(commandBuffer);

			VK_CHECK_RESULT(vkDeviceWaitIdle(device));
			ImGui_ImplVulkan_DestroyFontUploadObjects();
		}

		// Create ImGui Command Buffers
		uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;
		s_ImGuiCommandBuffers.resize(framesInFlight);
		for (uint32 i = 0; i < framesInFlight; i++)
			s_ImGuiCommandBuffers[i] = VulkanContext::GetCurrentDevice()->CreateSecondaryCommandBuffer();
	}

	void VulkanImGuiRenderer::Shutdown()
	{
		auto device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
		VK_CHECK_RESULT(vkDeviceWaitIdle(device));

		ImGui_ImplVulkan_Shutdown();

#ifdef HIGHLO_API_GLFW
		ImGui_ImplGlfw_Shutdown();
#endif // HIGHLO_API_GLFW
	}

	void VulkanImGuiRenderer::NewFrame()
	{
		ImGui_ImplVulkan_NewFrame();

	#ifdef HIGHLO_API_GLFW
		ImGui_ImplGlfw_NewFrame();
	#endif // HIGHLO_API_GLFW
	}

	void VulkanImGuiRenderer::Render()
	{
		ImGui::Render();
	}

	void VulkanImGuiRenderer::RenderDrawData()
	{
		Ref<VulkanSwapChain> swapChain = HLApplication::Get().GetWindow().GetSwapChain();

		VkClearValue clearValues[2];
		clearValues[0].color = { {0.1f, 0.1f,0.1f, 1.0f} };
		clearValues[1].depthStencil = { 1.0f, 0 };

		uint32 width = swapChain->GetWidth();
		uint32 height = swapChain->GetHeight();
		uint32 commandBufferIndex = swapChain->GetCurrentBufferIndex();

		VkCommandBufferBeginInfo drawCmdBufInfo = {};
		drawCmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		drawCmdBufInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		drawCmdBufInfo.pNext = nullptr;

		VkCommandBuffer drawCommandBuffer = swapChain->GetCurrentDrawCommandBuffer();
		VK_CHECK_RESULT(vkBeginCommandBuffer(drawCommandBuffer, &drawCmdBufInfo));

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.pNext = nullptr;
		renderPassBeginInfo.renderPass = swapChain->GetNativeRenderPass();
		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;
		renderPassBeginInfo.renderArea.extent.width = width;
		renderPassBeginInfo.renderArea.extent.height = height;
		renderPassBeginInfo.clearValueCount = 2; // Color + depth
		renderPassBeginInfo.pClearValues = clearValues;
		renderPassBeginInfo.framebuffer = swapChain->GetCurrentFramebuffer();

		vkCmdBeginRenderPass(drawCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);

		VkCommandBufferInheritanceInfo inheritanceInfo = {};
		inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		inheritanceInfo.renderPass = swapChain->GetNativeRenderPass();
		inheritanceInfo.framebuffer = swapChain->GetCurrentFramebuffer();

		VkCommandBufferBeginInfo cmdBufInfo = {};
		cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmdBufInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		cmdBufInfo.pInheritanceInfo = &inheritanceInfo;

		VK_CHECK_RESULT(vkBeginCommandBuffer(s_ImGuiCommandBuffers[commandBufferIndex], &cmdBufInfo));

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = (float)height;
		viewport.height = -(float)height;
		viewport.width = (float)width;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(s_ImGuiCommandBuffers[commandBufferIndex], 0, 1, &viewport);

		VkRect2D scissor = {};
		scissor.extent.width = width;
		scissor.extent.height = height;
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		vkCmdSetScissor(s_ImGuiCommandBuffers[commandBufferIndex], 0, 1, &scissor);

		ImDrawData *mainDrawData = ImGui::GetDrawData();
		ImGui_ImplVulkan_RenderDrawData(mainDrawData, s_ImGuiCommandBuffers[commandBufferIndex]);

		VK_CHECK_RESULT(vkEndCommandBuffer(s_ImGuiCommandBuffers[commandBufferIndex]));

		std::vector<VkCommandBuffer> commandBuffers;
		commandBuffers.push_back(s_ImGuiCommandBuffers[commandBufferIndex]);

		vkCmdExecuteCommands(drawCommandBuffer, uint32(commandBuffers.size()), commandBuffers.data());
		vkCmdEndRenderPass(drawCommandBuffer);
		VK_CHECK_RESULT(vkEndCommandBuffer(drawCommandBuffer));
	}
}

#endif // HIGHLO_API_VULKAN

