// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-03-04) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Engine/Graphics/Texture2D.h"
#include "Vulkan.h"
#include <vk_mem_alloc.h>

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

		VulkanTexture2D(const FileSystemPath &filePath, bool flipOnLoad = true);
		VulkanTexture2D(const glm::vec3 &rgb, TextureFormat format = TextureFormat::RGBA8);
		VulkanTexture2D(const glm::vec3 &rgb, uint32 width, uint32 height, TextureFormat format = TextureFormat::RGBA8);
		VulkanTexture2D(void *imgData, uint32 width, uint32 height, TextureFormat format);
		VulkanTexture2D(TextureFormat format, uint32 width, uint32 height, const void *data, TextureProperties props = TextureProperties());
		VulkanTexture2D(TextureFormat format, uint32 width, uint32 height);
		VulkanTexture2D(const TextureSpecification &spec);
		virtual ~VulkanTexture2D();

		virtual uint32 GetWidth() const override { return m_Specification.Width; };
		virtual uint32 GetHeight() const override { return m_Specification.Height; };
		virtual TextureFormat GetFormat() override { return m_Specification.Format; }
		virtual Allocator GetData() override;

		virtual void Release() override;
		virtual void Invalidate() override;
		virtual bool IsLoaded() const override { return m_Loaded; }

		virtual void Resize(const glm::uvec2 &size) override;
		virtual void Resize(const uint32 width, const uint32 height) override;

		virtual void Lock() override;
		virtual void Unlock() override;

		virtual void CreatePerLayerImageViews() override;
		virtual void CreateSampler(TextureProperties properties) override;
		virtual void CreatePerSpecificLayerImageViews(const std::vector<uint32> &layerIndices) override;

		virtual TextureSpecification &GetSpecification() override { return m_Specification; }
		virtual const TextureSpecification &GetSpecification() const override { return m_Specification; }

		virtual void UpdateResourceData() override;
		virtual void UpdateResourceData(void *data) override;
		virtual void WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba) override;
		virtual glm::ivec4 ReadPixel(uint32 row, uint32 column) override;
		virtual uint32 GetMipLevelCount() override;
		virtual std::pair<uint32, uint32> GetMipSize(uint32 mip) override;
		virtual void GenerateMips(bool readonly = false) override;
		virtual float GetAspectRatio() const override { return (float)m_Specification.Width / (float)m_Specification.Height; }

		virtual HLRendererID GetSamplerRendererID() const override { return m_SamplerRendererID; }

		virtual void SetData(void *data, uint32 data_size) override;

		virtual void Bind(uint32 slot) const override {}
		virtual void Unbind(uint32 slot) const override {}

		// Vulkan-specific
		void CopyToHostBuffer(Allocator &buffer);
		const VkDescriptorImageInfo &GetDescriptorInfo() const { return m_DescriptorImageInfo; }

		void UpdateDescriptor();

		VkImageView GetLayerImageView(uint32 layer)
		{
			HL_ASSERT(layer < m_PerLayerImageViews.size());
			return m_PerLayerImageViews[layer];
		}

		VkImageView GetMipImageView(uint32 mip);

		VulkanTextureInfo &GetTextureInfo() { return m_Info; }
		const VulkanTextureInfo &GetTextureInfo() const { return m_Info; }

		// Debug
		static const std::map<VkImage, WeakRef<VulkanTexture2D>> &GetImageRefs();

	private:

		void InvalidateVulkanData();
		void ReleaseVulkanData();

	private:

		HLRendererID m_RendererID = 0;
		HLRendererID m_SamplerRendererID = 0;
		TextureSpecification m_Specification;

		bool m_Loaded = false;
		bool m_Locked = false;

		FileSystemPath m_FilePath;
		Allocator m_Buffer;

		VulkanTextureInfo m_Info;
		VkDeviceSize m_GPUAllocationSize;

		std::vector<VkImageView> m_PerLayerImageViews;
		std::map<uint32, VkImageView> m_PerMipImageViews;
		VkDescriptorImageInfo m_DescriptorImageInfo = {};
	};
}

#endif // HIGHLO_API_VULKAN

