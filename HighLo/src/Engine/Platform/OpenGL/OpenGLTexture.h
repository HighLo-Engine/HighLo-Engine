// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.2 (2021-09-21) Added Create method that also contains the pixel data
//     - 1.1 (2021-09-15) Changed Name implementation
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Renderer/Texture.h"
#include "Engine/Core/Allocator.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

namespace highlo
{
	class OpenGLTexture2D : public Texture2D
	{
	public:

		OpenGLTexture2D(const FileSystemPath &filePath, TextureFormat format = TextureFormat::RGBA8, bool flipOnLoad = true);
		OpenGLTexture2D(const glm::vec3 &rgb, TextureFormat format = TextureFormat::RGBA8);
		OpenGLTexture2D(const glm::vec3 &rgb, uint32 width, uint32 height, TextureFormat format = TextureFormat::RGBA8);
		OpenGLTexture2D(void *imgData, uint32 width, uint32 height, TextureFormat format);
		OpenGLTexture2D(TextureFormat format, uint32 width, uint32 height, const void *data, TextureProperties props = TextureProperties());
		OpenGLTexture2D(TextureFormat format, uint32 width, uint32 height, bool waitUntilRenderQueueSubmission = true);
		~OpenGLTexture2D();

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

		virtual TextureSpecification &GetSpecification() override { return m_Specification; }
		virtual const TextureSpecification &GetSpecification() const override { return m_Specification; }

		virtual void UpdateResourceData() override;
		virtual void UpdateResourceData(void *data) override;
		virtual void WritePixel(uint32 row, uint32 column, const glm::ivec4& rgba) override;
		virtual glm::ivec4 ReadPixel(uint32 row, uint32 column) override;
		virtual uint32 GetMipLevelCount() override;

		virtual HLRendererID GetSamplerRendererID() const override { return m_SamplerRendererID; }

		virtual void Bind(uint32 slot) const override;
		virtual void Unbind(uint32 slot) const override;

	private:

		Allocator m_Buffer;
		HLRendererID m_SamplerRendererID = 0;
		GLenum m_InternalFormat, m_DataFormat;
		TextureSpecification m_Specification;
		bool m_Locked = false;
		bool m_Loaded = false;
	};

	class OpenGLTexture3D : public Texture3D
	{
	public:

		OpenGLTexture3D(const std::vector<HLString> &filePaths, bool flipOnLoad = false);
		OpenGLTexture3D(TextureFormat format, uint32 width, uint32 height, const void *data);
		virtual ~OpenGLTexture3D();

		virtual uint32 GetWidth() const override { return m_Specification.Width; }
		virtual uint32 GetHeight() const override { return m_Specification.Height; }
		virtual TextureFormat GetFormat() override { return m_Specification.Format; }
		virtual Allocator GetData() override;

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

		virtual TextureSpecification &GetSpecification() override { return m_Specification; }
		virtual const TextureSpecification &GetSpecification() const override { return m_Specification; }

		virtual void Bind(uint32 slot) const override;
		virtual void Unbind(uint32 slot) const override;

	private:

		Allocator m_Buffer;
		TextureSpecification m_Specification;
		bool m_Locked = false;
		bool m_Loaded = false;
	};
}

#endif

