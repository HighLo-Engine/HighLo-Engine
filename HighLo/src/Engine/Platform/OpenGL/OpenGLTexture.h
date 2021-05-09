#pragma once

#include "Engine/Renderer/Texture.h"
#include "Engine/Core/HLAllocator.h"

#ifdef HIGHLO_API_OPENGL

#include <glad/glad.h>

namespace highlo
{
	class OpenGLTexture2D : public Texture2D
	{
	public:

		static OpenGLTexture2D* LoadFromFile(const HLString& filepath, TextureFormat format = TextureFormat::RGBA8, bool flip_on_load = true);
		static OpenGLTexture2D* CreateFromColor(const glm::vec3& rgb, TextureFormat format = TextureFormat::RGBA8);
		static OpenGLTexture2D* CreateFromColor(const glm::vec3& rgb, uint32 width, uint32 height, TextureFormat format = TextureFormat::RGBA8);

		OpenGLTexture2D(void* img_data, uint32 width, uint32 height, TextureFormat format);
		OpenGLTexture2D(TextureFormat format, uint32 width, uint32 height);
		~OpenGLTexture2D();

		virtual uint32 GetWidth() const override { return m_Width; };
		virtual uint32 GetHeight() const override { return m_Height; };
		virtual void* GetData() const override;

		virtual void Release() override;
		virtual void Invalidate() override;

		// Inherited via Texture
		virtual void UpdateResourceData() override;
		virtual void UpdateResourceData(void* data) override;
		virtual void WritePixel(uint32 row, uint32 column, const glm::ivec4& rgba) override;
		virtual glm::ivec4 ReadPixel(uint32 row, uint32 column) override;
		virtual uint32 GetMipLevelCount() override;
		virtual HLRendererID GetRendererID() const override;

		virtual void Bind(uint32 slot) const override;

	private:

		void* m_ImageData = nullptr;
		uint32 m_Width = 0, m_Height = 0;
		uint32 m_ID = 0;
		GLenum m_InternalFormat, m_DataFormat;
	};

	class OpenGLTexture3D : public Texture3D
	{
	public:
		OpenGLTexture3D(const std::vector<HLString> &filePaths);
		OpenGLTexture3D(TextureFormat format, uint32 width, uint32 height, const void* data);
		virtual ~OpenGLTexture3D();

		virtual uint32 GetWidth() const override;
		virtual uint32 GetHeight() const override;
		virtual void *GetData() const override;

		virtual void Release() override;
		virtual void Invalidate() override;

		virtual void WritePixel(uint32 row, uint32 column, const glm::ivec4 &rgba) override;
		virtual glm::ivec4 ReadPixel(uint32 row, uint32 column) override;
		virtual void UpdateResourceData(void *data) override;
		virtual void UpdateResourceData() override;
		virtual uint32 GetMipLevelCount() override;
		virtual HLRendererID GetRendererID() const override;

		virtual void Bind(uint32 slot) const override;

	private:
		uint32 m_ID = 0;
		uint32 m_Width = 0, m_Height = 0;
		TextureFormat m_Format;
		Allocator m_Buffer;
	};
}

#endif
