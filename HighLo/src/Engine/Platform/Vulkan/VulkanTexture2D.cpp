// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanTexture2D.h"

#include <stb_image.h>

#ifdef HIGHLO_API_VULKAN

#include "Engine/Utils/ImageUtils.h"

namespace highlo
{
	namespace utils
	{
		static VkSamplerAddressMode VulkanSamplerWrap(TextureWrap wrap)
		{
			switch (wrap)
			{
				case TextureWrap::Clamp:   return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
				case TextureWrap::Repeat:  return VK_SAMPLER_ADDRESS_MODE_REPEAT;
			}

			HL_ASSERT(false, "Unknown wrap mode");
			return (VkSamplerAddressMode)0;
		}

		static VkFilter VulkanSamplerFilter(TextureFilter filter)
		{
			switch (filter)
			{
				case TextureFilter::Linear:		return VK_FILTER_LINEAR;
				case TextureFilter::Nearest:	return VK_FILTER_NEAREST;
				case TextureFilter::Cubic:		return VK_FILTER_CUBIC_IMG;
			}

			HL_ASSERT(false, "Unknown filter");
			return (VkFilter)0;
		}
	}

	VulkanTexture2D::VulkanTexture2D(const FileSystemPath &filePath, TextureFormat format, bool flipOnLoad)
		: m_FilePath(filePath)
	{
		
	}

	VulkanTexture2D::VulkanTexture2D(const glm::vec3 &rgb, TextureFormat format)
	{
	}

	VulkanTexture2D::VulkanTexture2D(const glm::vec3 &rgb, uint32 width, uint32 height, TextureFormat format)
	{
	}

	VulkanTexture2D::VulkanTexture2D(void *imgData, uint32 width, uint32 height, TextureFormat format)
	{
	}

	VulkanTexture2D::VulkanTexture2D(TextureFormat format, uint32 width, uint32 height, const void *data, TextureProperties props)
	{
	}

	VulkanTexture2D::VulkanTexture2D(TextureFormat format, uint32 width, uint32 height)
	{
	}

	VulkanTexture2D::VulkanTexture2D(const TextureSpecification &spec)
	{
	}
	
	VulkanTexture2D::~VulkanTexture2D()
	{
	}

	void VulkanTexture2D::Release()
	{

	}

	void VulkanTexture2D::Invalidate()
	{

	}

	void VulkanTexture2D::Resize(const glm::uvec2 &size)
	{

	}

	void VulkanTexture2D::Resize(const uint32 width, const uint32 height)
	{

	}

	void VulkanTexture2D::Lock()
	{

	}

	void VulkanTexture2D::Unlock()
	{
	
	}

	void VulkanTexture2D::CreatePerLayerImageViews()
	{
	}

	void VulkanTexture2D::CreateSampler(TextureProperties properties)
	{
	}

	void VulkanTexture2D::CreatePerSpecificLayerImageViews(const std::vector<uint32> &layerIndices)
	{
	}

	void VulkanTexture2D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
	{

	}

	glm::ivec4 VulkanTexture2D::ReadPixel(uint32 row, uint32 column)
	{
		return {};
	}

	void VulkanTexture2D::UpdateResourceData(void *data)
	{

	}

	void VulkanTexture2D::UpdateResourceData()
	{

	}

	uint32 VulkanTexture2D::GetMipLevelCount()
	{
		return 0;
	}

	std::pair<uint32, uint32> VulkanTexture2D::GetMipSize(uint32 mip)
	{
		return {};
	}

	void VulkanTexture2D::GenerateMips(bool readonly)
	{
	}

	VkImageView VulkanTexture2D::GetLayerImageView(uint32 layer)
	{
		// TODO
		VkImageView view;
		return view;
	}

	VkImageView VulkanTexture2D::GetMipImageView(uint32 mip)
	{
		// TODO
		VkImageView view;
		return view;
	}
}

#endif // HIGHLO_API_VULKAN

