#pragma once

#include "Engine/Renderer/Texture.h"

#ifdef HIGHLO_API_VULKAN

#include "Vulkan.h"
#include "VulkanAllocator.h"
#include "VulkanUtils.h"

namespace highlo
{
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

		void GenerateMips(bool readOnly = false);
		VkImageView CreateImageViewSingleMip(uint32 mip);
		const VkDescriptorImageInfo &GetVulkanDescriptorInfo() const { return m_DescriptorImageInfo; }

	private:

		Allocator m_Buffer;
		TextureSpecification m_Specification;
		bool m_Locked = false;
		bool m_Loaded = false;
		bool m_MipsGenerated = false;

		VmaAllocation m_MemoryAlloc;
		VkImage m_Image;
		VkDescriptorImageInfo m_DescriptorImageInfo = {};
	};
}

#endif // HIGHLO_API_VULKAN

