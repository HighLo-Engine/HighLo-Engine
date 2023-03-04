// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-03-04) initial release
//

#pragma once

#ifdef HIGHLO_API_VULKAN

#include "Engine/Graphics/Texture3D.h"
#include "Vulkan.h"
#include <vk_mem_alloc.h>

namespace highlo
{
	class VulkanTexture3D : public Texture3D
	{
	public:

		VulkanTexture3D(const FileSystemPath &filePath, bool flipOnLoad = false);
		VulkanTexture3D(TextureFormat format, uint32 width, uint32 height, const void *data);
		virtual ~VulkanTexture3D();

		virtual uint32 GetWidth() const override { return m_Specification.Width; }
		virtual uint32 GetHeight() const override { return m_Specification.Height; }
		virtual TextureFormat GetFormat() override { return m_Specification.Format; }
		virtual Allocator GetData() override;

		virtual void Release() override;
		virtual void Invalidate() override;
		virtual bool IsLoaded() const override { return m_Loaded; }

		virtual void Resize(const glm::uvec2 &size) override;
		virtual void Resize(const uint32 width, const uint32 height) override;

		virtual void Lock() override;
		virtual void Unlock() override;

		virtual void WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba) override;
		virtual glm::ivec4 ReadPixel(uint32 row, uint32 column) override;
		virtual void UpdateResourceData(void *data) override;
		virtual void UpdateResourceData() override;
		virtual uint32 GetMipLevelCount() override;
		virtual std::pair<uint32, uint32> GetMipSize(uint32 mip) override;
		virtual void GenerateMips(bool readonly = false) override;
		virtual float GetAspectRatio() const override { return (float)m_Specification.Width / (float)m_Specification.Height; }

		virtual TextureSpecification &GetSpecification() override { return m_Specification; }
		virtual const TextureSpecification &GetSpecification() const override { return m_Specification; }

		virtual void Bind(uint32 slot) const override {}
		virtual void Unbind(uint32 slot) const override {}

		// Vulkan-specific
		const VkDescriptorImageInfo &GetVulkanDescriptorInfo() const { return m_DescriptorImageInfo; }
		VkImageView CreateImageViewSingleMip(uint32 mip);

		void CopyToHostBuffer(Allocator &buffer);
		void CopyFromBuffer(const Allocator &buffer, uint32 mips);

	private:

		HLRendererID m_RendererID = 0;
		TextureSpecification m_Specification;
		Allocator m_Buffer;
		FileSystemPath m_FilePath;

		bool m_Loaded = false;
		bool m_Locked = false;
		bool m_MipsGenerated = false;

		VmaAllocation m_MemoryAlloc;
		uint64 m_GPUAllocationSize = 0;

		VkImage m_Image{ nullptr };
		VkDescriptorImageInfo m_DescriptorImageInfo = {};
	};
}

#endif // HIGHLO_API_VULKAN

