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
#include "Engine/ImGui/ImGui/backends/imgui_impl_vulkan.h"

#include "Engine/Renderer/Renderer.h"
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
		VulkanImGuiRenderer *instance = this;
		Renderer::Submit([instance, window]()
		{
			auto vulkanContext = VulkanContext::Get();
			auto device = VulkanContext::GetCurrentDevice()->GetNativeDevice();
			VkDescriptorPoolCreateInfo poolInfo = {};
			ImGui_ImplVulkan_InitInfo initInfo = {};
			Ref<VulkanSwapChain> swapChain = HLApplication::Get().GetWindow().GetSwapChain().As<VulkanSwapChain>();
			VkDescriptorPool descriptorPool;
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

			poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			poolInfo.maxSets = 100 * (uint32)IM_ARRAYSIZE(poolSizes);
			poolInfo.poolSizeCount = (uint32)IM_ARRAYSIZE(poolSizes);
			poolInfo.pPoolSizes = poolSizes;
			VK_CHECK_RESULT(vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool));

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

			// Init ImGui
		#ifdef HIGHLO_API_GLFW
			ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)window->GetNativeHandle(), true);
		#else
			ImGui_ImplWin32_Init(window->GetNativeHandle(), window->GetNativeContext());
		#endif // HIGHLO_API_GLFW

			ImGui_ImplVulkan_Init(&initInfo, swapChain->GetNativeRenderPass());


			uint32 framesInFlight = Renderer::GetConfig().FramesInFlight;
			s_ImGuiCommandBuffers.resize(framesInFlight);
			for (uint32 i = 0; i < framesInFlight; ++i)
				s_ImGuiCommandBuffers[i] = VulkanContext::GetCurrentDevice()->CreateSecondayCommandBuffer();
		});
	}

	void VulkanImGuiRenderer::Shutdown()
	{
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
		Ref<VulkanSwapChain> swapChain = HLApplication::Get().GetWindow().GetSwapChain();
		std::vector<VkCommandBuffer> commandBuffers;
		VkClearValue clearValues[2];
		uint32 width = swapChain->GetWidth();
		uint32 height = swapChain->GetHeight();
		uint32 commandBufferIndex = swapChain->GetCurrentBufferIndex();
		VkCommandBufferBeginInfo drawCmdBufInfo = {};
		VkCommandBufferBeginInfo cmdBufInfo = {};
		VkCommandBufferInheritanceInfo inheritenceInfo = {};
		VkCommandBuffer drawCmdBuffer;
		VkViewport viewport = {};
		VkRect2D scissor = {};
		VkRenderPassBeginInfo renderPassBeginInfo = {};

		clearValues[0].color = { { 0.1f, 0.1f, 0.1f, 1.0f } };
		clearValues[1].depthStencil = { 1.0f, 0 };

		// begin Draw buffer

		drawCmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		drawCmdBufInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		drawCmdBufInfo.pNext = nullptr;

		drawCmdBuffer = swapChain->GetCurrentDrawCommandBuffer();
		VK_CHECK_RESULT(vkBeginCommandBuffer(drawCmdBuffer, &drawCmdBufInfo));

		// Renderpass
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.pNext = nullptr;
		renderPassBeginInfo.renderPass = swapChain->GetNativeRenderPass();
		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;
		renderPassBeginInfo.renderArea.extent.width = width;
		renderPassBeginInfo.renderArea.extent.height = height;
		renderPassBeginInfo.clearValueCount = 2;
		renderPassBeginInfo.pClearValues = clearValues;
		renderPassBeginInfo.framebuffer = swapChain->GetCurrentFramebuffer();
		vkCmdBeginRenderPass(drawCmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);

		inheritenceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		inheritenceInfo.renderPass = swapChain->GetNativeRenderPass();
		inheritenceInfo.framebuffer = swapChain->GetCurrentFramebuffer();

		cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmdBufInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		cmdBufInfo.pInheritanceInfo = &inheritenceInfo;
		VK_CHECK_RESULT(vkBeginCommandBuffer(s_ImGuiCommandBuffers[commandBufferIndex], &cmdBufInfo));

		viewport.x = 0.0f;
		viewport.y = (float)height;
		viewport.width = (float)width;
		viewport.height = -(float)height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(s_ImGuiCommandBuffers[commandBufferIndex], 0, 1, &viewport);

		scissor.extent.width = width;
		scissor.extent.height = height;
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		vkCmdSetScissor(s_ImGuiCommandBuffers[commandBufferIndex], 0, 1, &scissor);

		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), s_ImGuiCommandBuffers[commandBufferIndex]);
		VK_CHECK_RESULT(vkEndCommandBuffer(s_ImGuiCommandBuffers[commandBufferIndex]));

		commandBuffers.push_back(s_ImGuiCommandBuffers[commandBufferIndex]);
		vkCmdExecuteCommands(drawCmdBuffer, (uint32)commandBuffers.size(), commandBuffers.data());
		VK_CHECK_RESULT(vkEndCommandBuffer(drawCmdBuffer));
	}
}

#endif // HIGHLO_API_VULKAN

