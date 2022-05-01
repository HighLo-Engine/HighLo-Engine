// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Graphics/Texture2D.h"

#ifdef HIGHLO_API_DX11

#include "Engine/Platform//DX11/DX11Resources.h"

namespace highlo
{
	class DX11Texture2D : public Texture2D
	{
	friend class DX11FrameBuffer;

	public:

		DX11Texture2D(const FileSystemPath &filePath, TextureFormat format = TextureFormat::RGBA8, bool flipOnLoad = false);
		DX11Texture2D(const glm::vec3 &rgb, TextureFormat format = TextureFormat::RGBA8);
		DX11Texture2D(const glm::vec3 &rgb, uint32 width, uint32 height, TextureFormat format = TextureFormat::RGBA8);
		DX11Texture2D(TextureFormat format, uint32 width, uint32 height);
		DX11Texture2D(TextureFormat format, uint32 width, uint32 height, const void *data, TextureProperties props = TextureProperties());
		DX11Texture2D(const TextureSpecification &specification);
		virtual ~DX11Texture2D();

		virtual uint32 GetWidth() const override { return m_Specification.Width; }
		virtual uint32 GetHeight() const override { return m_Specification.Height; }
		virtual Allocator GetData() override;

		virtual void Bind(uint32 slot) const override;
		virtual void Unbind(uint32 slot) const override;

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

		virtual TextureFormat GetFormat() override { return m_Specification.Format; }

		virtual TextureSpecification &GetSpecification() override { return m_Specification; }
		virtual const TextureSpecification &GetSpecification() const override { return m_Specification; }

		// Texture2D specific

		virtual HLRendererID GetSamplerRendererID() const override { return m_SamplerRendererID; }
		virtual void CreatePerLayerImageViews() override;
		virtual void CreateSampler(TextureProperties properties) override;
		virtual void CreatePerSpecificLayerImageViews(const std::vector<uint32> &layerIndices) override;

	private:

		Allocator m_Buffer;
		FileSystemPath m_FilePath;
		HLRendererID m_SamplerRendererID = 0;
		TextureSpecification m_Specification;
		bool m_Locked = false;
		bool m_Loaded = false;
	};
}

#endif // HIGHLO_API_DX11

