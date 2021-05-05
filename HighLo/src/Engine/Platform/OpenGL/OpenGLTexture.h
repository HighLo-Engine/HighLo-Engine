#pragma once
#include "Engine/Renderer/Texture.h"

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
		~OpenGLTexture2D();

		virtual uint32 GetWidth() const override { return m_Width; };
		virtual uint32 GetHeight() const override { return m_Height; };
		virtual void* GetData() const override;

		// Inherited via Texture
		virtual void UpdateResourceData() override;
		virtual void UpdateResourceData(void* data) override;
		virtual void WritePixel(uint32 row, uint32 column, const glm::ivec4& rgba) override;
		virtual glm::ivec4 ReadPixel(uint32 row, uint32 column) override;

		virtual void Bind(uint32 slot) const override;

	private:
		void* m_ImageData = nullptr;
		uint32 m_Width = 0, m_Height = 0;
		uint32 m_ID = 0;
		GLenum m_InternalFormat, m_DataFormat;
	};
}

#endif
