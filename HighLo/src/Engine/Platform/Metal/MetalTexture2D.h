// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2023-02-19) initial release
//

#pragma once

#ifdef HIGHLO_API_METAL

#include "Engine/Graphics/Texture2D.h"

namespace highlo
{
	class MetalTexture2D : public Texture2D
	{
	public:

		MetalTexture2D(const FileSystemPath &filePath, bool flipOnLoad = true);
		MetalTexture2D(const glm::vec3 &rgb, TextureFormat format = TextureFormat::RGBA8);
		MetalTexture2D(const glm::vec3 &rgb, uint32 width, uint32 height, TextureFormat format = TextureFormat::RGBA8);
		MetalTexture2D(void *imgData, uint32 width, uint32 height, TextureFormat format);
		MetalTexture2D(TextureFormat format, uint32 width, uint32 height, const void *data, TextureProperties props = TextureProperties());
		MetalTexture2D(TextureFormat format, uint32 width, uint32 height);
		MetalTexture2D(const TextureSpecification &spec);
		virtual ~MetalTexture2D();

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

		virtual void Bind(uint32 slot) const override;
		virtual void Unbind(uint32 slot) const override;

	private:

		Allocator m_Buffer;
		HLRendererID m_SamplerRendererID = 0;
		TextureSpecification m_Specification;
		FileSystemPath m_FilePath = "";
		bool m_Locked = false;
		bool m_Loaded = false;
	};
}

#endif // HIGHLO_API_METAL

