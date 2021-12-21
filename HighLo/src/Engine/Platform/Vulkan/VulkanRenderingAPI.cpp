// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanRenderingAPI.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	void VulkanRenderingAPI::Init()
	{
	}
	
	void VulkanRenderingAPI::Shutdown()
	{
	}
	
	void VulkanRenderingAPI::ClearScreenColor(const glm::vec4 &color)
	{
	}
	
	void VulkanRenderingAPI::ClearScreenBuffers()
	{
	}
	
	void VulkanRenderingAPI::DrawIndexed(Ref<VertexArray> &va, PrimitiveType type)
	{
	}
	
	void VulkanRenderingAPI::DrawIndexed(uint32 indexCount, PrimitiveType type, bool depthTest)
	{
	}
	
	void VulkanRenderingAPI::DrawInstanced(Ref<VertexArray> &va, uint32 count, PrimitiveType type)
	{
	}
	
	void VulkanRenderingAPI::DrawIndexedControlPointPatchList(Ref<VertexArray> &va, PrimitiveType type)
	{
	}
	
	void VulkanRenderingAPI::BeginFrame()
	{
	}
	
	void VulkanRenderingAPI::EndFrame()
	{
	}
	
	void VulkanRenderingAPI::SetWireframe(bool wf)
	{
	}
	
	void VulkanRenderingAPI::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height)
	{
	}
	
	void VulkanRenderingAPI::SetBlendMode(bool bEnabled)
	{
	}
	
	void VulkanRenderingAPI::SetMultiSample(bool bEnabled)
	{
	}
	
	void VulkanRenderingAPI::SetDepthTest(bool bEnabled)
	{
	}
	
	void VulkanRenderingAPI::SetLineThickness(float thickness)
	{
	}

	Ref<Environment> VulkanRenderingAPI::CreateEnvironment(const HLString &path, uint32 cubemapSize, uint32 irradianceMapSize)
	{
		// TODO
		return nullptr;
	}
}

#endif // HIGHLO_API_VULKAN

