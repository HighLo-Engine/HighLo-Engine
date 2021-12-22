// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-11-23) initial release
//

#pragma once

#include "Engine/Renderer/Texture.h"

#ifdef HIGHLO_API_VULKAN

#include "Vulkan.h"
#include "VulkanAllocator.h"
#include "VulkanUtils.h"

namespace highlo
{
	struct VulkanTextureInfo
	{
		VkImage Image = nullptr;
		VkImageView ImageView = nullptr;
		VkSampler Sampler = nullptr;
		VmaAllocation MemoryAlloc = nullptr;
	};

	class VulkanTexture2D : public Texture2D
	{
	public:

		VulkanTexture2D(const FileSystemPath &filePath, TextureFormat format = TextureFormat::RGBA8, bool flipOnLoad = true);
		VulkanTexture2D(const glm::vec3 &rgb, TextureFormat format = TextureFormat::RGBA8);
		VulkanTexture2D(const glm::vec3 &rgb, uint32 width, uint32 height, TextureFormat format = TextureFormat::RGBA8);
		VulkanTexture2D(void *imgData, uint32 width, uint32 height, TextureFormat format);
		VulkanTexture2D(TextureFormat format, uint32 width, uint32 height, const void *data, TextureProperties props = TextureProperties());
		VulkanTexture2D(TextureFormat format, uint32 width, uint32 height);
		virtual ~VulkanTexture2D();

		virtual uint32 GetWidth() const override { return m_Specification.Width; }
		virtual uint32 GetHeight() const override { return m_Specification.Height; }
		virtual Allocator GetData() override;

		virtual void Bind(uint32 slot) const override;
		virtual void Unbind(uint32 slot) const override;

		virtual void Release() override;
		virtual void Invalidate() override;
		virtual bool IsLoaded() const override { return m_Loaded; }

		virtual void Lock() override;
		virtual void Unlock() override;

		virtual void WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba) override;
		virtual glm::ivec4 ReadPixel(uint32 row, uint32 column) override;
		virtual void UpdateResourceData(void *data) override;
		virtual void UpdateResourceData() override;
		virtual uint32 GetMipLevelCount() override;

		virtual TextureFormat GetFormat() override { return m_Specification.Format; }
		virtual TextureSpecification &GetSpecification() override { return m_Specification; }
		virtual const TextureSpecification &GetSpecification() const override { return m_Specification; }

		// Specific to Texture2D

		virtual HLRendererID GetSamplerRendererID() const override { return m_SamplerRendererID; }

		virtual void CreatePerLayerImageViews() override;
		virtual void CreateSampler(TextureProperties properties) override;

		VulkanTextureInfo &GetTextureInfo() { return m_Info; }
		const VulkanTextureInfo &GetTextureInfo() const { return m_Info; }

		const VkDescriptorImageInfo &GetDescriptor() { return m_DescriptorImageInfo; }

		void CreatePerSpecificLayerImageViews(const std::vector<uint32> &layerIndices);
		virtual VkImageView GetLayerImageView(uint32 layer)
		{
			HL_ASSERT(layer < m_PerLayerImageViews.size());
			return m_PerLayerImageViews[layer];
		}

	private:

		Allocator m_Buffer;
		HLRendererID m_SamplerRendererID = 0;
		TextureSpecification m_Specification;
		bool m_Locked = false;
		bool m_Loaded = false;

		VulkanTextureInfo m_Info;
		VkDescriptorImageInfo m_DescriptorImageInfo = {};
		std::vector<VkImageView> m_PerLayerImageViews;
		std::map<uint32, VkImageView> m_MipImageViews;
	};

	class VulkanTexture3D : public Texture3D
	{
	public:

		VulkanTexture3D(const FileSystemPath &filepath, bool flipOnLoad = false);
		VulkanTexture3D(TextureFormat format, uint32 width, uint32 height, const void *data = nullptr);
		virtual ~VulkanTexture3D();

		virtual uint32 GetWidth() const override { return m_Specification.Width; }
		virtual uint32 GetHeight() const override { return m_Specification.Height; }
		virtual Allocator GetData() override;

		void Bind(uint32 slot) const override;
		void Unbind(uint32 slot) const override;

		virtual void Release() override;
		virtual void Invalidate() override;
		virtual bool IsLoaded() const override { return m_Loaded; }

		virtual void Lock() override;
		virtual void Unlock() override;

		virtual void WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba) override;
		virtual glm::ivec4 ReadPixel(uint32 row, uint32 column) override;
		virtual void UpdateResourceData(void *data) override;
		virtual void UpdateResourceData() override;
		virtual uint32 GetMipLevelCount() override;

		virtual TextureFormat GetFormat() override { return m_Specification.Format; }
		virtual TextureSpecification &GetSpecification() override { return m_Specification; }
		virtual const TextureSpecification &GetSpecification() const override { return m_Specification; }

		// Specific to Texture3D

	private:

		Allocator m_Buffer;
		TextureSpecification m_Specification;
		bool m_Locked = false;
		bool m_Loaded = false;
	};
}

#endif // HIGHLO_API_VULKAN

