// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-11-19) initial release
//

#pragma once

#include "Engine/Graphics/Texture3D.h"

#include "Vulkan.h"

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
		virtual Allocator GetData() override { return m_LocalData; }

		virtual void Bind(uint32 slot) const override {}
		virtual void Unbind(uint32 slot) const override {}

		virtual void Release() override;
		virtual void Invalidate() override;
		virtual bool IsLoaded() const override;

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
		virtual float GetAspectRatio() const override { return m_Specification.Width / m_Specification.Height; }

		virtual TextureFormat GetFormat() override { return m_Specification.Format; }

		virtual TextureSpecification &GetSpecification() override { return m_Specification; }
		virtual const TextureSpecification &GetSpecification() const override { return m_Specification; }

	private:

		TextureSpecification m_Specification;
		Allocator m_LocalData;
	};
}

