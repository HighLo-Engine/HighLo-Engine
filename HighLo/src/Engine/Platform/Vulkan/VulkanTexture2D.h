#pragma once

#include "Engine/Graphics/Texture2D.h"

#ifdef HIGHLO_API_VULKAN

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace highlo
{
	struct VulkanImageInfo
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
		VulkanTexture2D(const TextureSpecification &spec);
		virtual ~VulkanTexture2D();

		virtual uint32 GetWidth() const override { return m_Specification.Width; };
		virtual uint32 GetHeight() const override { return m_Specification.Height; };
		virtual TextureFormat GetFormat() override { return m_Specification.Format; }
		virtual Allocator GetData() override;

		virtual void Release() override;
		virtual void Invalidate() override;
		virtual bool IsLoaded() const override { return m_Loaded; }

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
		virtual void GenerateMips() override;

		virtual HLRendererID GetSamplerRendererID() const override { return m_SamplerRendererID; }

		virtual void Bind(uint32 slot) const override;
		virtual void Unbind(uint32 slot) const override;

		// Vulkan-specific
		const VkDescriptorImageInfo &GetDescriptorInfo() const { return m_DescriptorImageInfo; }

		VulkanImageInfo &GetTextureInfo() { return m_Info; }
		const VulkanImageInfo &GetTextureInfo() const { return m_Info; }

		VkImageView GetLayerImageView(uint32 layer);
		VkImageView GetMipImageView(uint32 mip);

	private:

		Allocator m_Buffer;
		HLRendererID m_SamplerRendererID = 0;
		TextureSpecification m_Specification;
		bool m_Locked = false;
		bool m_Loaded = false;

		VkDescriptorImageInfo m_DescriptorImageInfo = {};
		VulkanImageInfo m_Info;
	};
}

#endif // HIGHLO_API_VULKAN

