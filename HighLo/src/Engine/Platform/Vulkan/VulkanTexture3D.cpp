// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "VulkanTexture3D.h"

#ifdef HIGHLO_API_VULKAN

namespace highlo
{
	VulkanTexture3D::VulkanTexture3D(const FileSystemPath &filepath, bool flipOnLoad)
	{

	}

	VulkanTexture3D::VulkanTexture3D(TextureFormat format, uint32 width, uint32 height, const void *data)
	{

	}

	VulkanTexture3D::~VulkanTexture3D()
	{

	}

	void VulkanTexture3D::Release()
	{

	}

	void VulkanTexture3D::Invalidate()
	{

	}

	bool VulkanTexture3D::IsLoaded() const
	{
		return false;
	}

	void VulkanTexture3D::Resize(const glm::uvec2 &size)
	{

	}

	void VulkanTexture3D::Resize(const uint32 width, const uint32 height)
	{

	}

	void VulkanTexture3D::Lock()
	{

	}

	void VulkanTexture3D::Unlock()
	{

	}

	void VulkanTexture3D::WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba)
	{

	}

	glm::ivec4 VulkanTexture3D::ReadPixel(uint32 row, uint32 column)
	{
		return {};
	}

	void VulkanTexture3D::UpdateResourceData(void *data)
	{

	}

	void VulkanTexture3D::UpdateResourceData()
	{

	}

	uint32 VulkanTexture3D::GetMipLevelCount()
	{
		return 0;
	}

	std::pair<uint32, uint32> VulkanTexture3D::GetMipSize(uint32 mip)
	{
		return {};
	}

	void VulkanTexture3D::GenerateMips(bool readonly)
	{

	}
}

#endif // HIGHLO_API_VULKAN

