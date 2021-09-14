// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once
#include "Engine/Renderer/Texture.h"

#ifdef HIGHLO_API_DX11

#include "Engine/Platform//DX11/DX11Resources.h"

namespace highlo
{
	class DX11Texture2D : public Texture2D
	{
		friend class DX11FrameBuffer;

	public:
		static DX11Texture2D* LoadFromFile(const HLString& filepath, TextureFormat format = TextureFormat::RGBA8, bool flip_on_load = true);
		static DX11Texture2D* CreateFromColor(const glm::vec3& rgb, TextureFormat format = TextureFormat::RGBA8);
		static DX11Texture2D* CreateFromColor(const glm::vec3& rgb, uint32 width, uint32 height, TextureFormat format = TextureFormat::RGBA8);

		DX11Texture2D(void* img_data, uint32 width, uint32 height, TextureFormat format);
		~DX11Texture2D();

		virtual uint32 GetWidth() const override { return m_Width; };
		virtual uint32 GetHeight() const override { return m_Height; };
		virtual void* GetData() const override;

		// Inherited via Texture
		virtual void UpdateResourceData() override;
		virtual void UpdateResourceData(void* data) override;
		virtual void WritePixel(uint32 row, uint32 column, const glm::ivec4& rgba) override;
		virtual glm::ivec4 ReadPixel(uint32 row, uint32 column) override;
		virtual uint32 GetMipLevelCount() override;
		virtual HLRendererID GetRendererID() override;

		virtual void Bind(uint32 slot) const override;

	private:
		void* m_ImageData = nullptr;
		uint32 m_Width = 0, m_Height = 0;
		ComPtr<ID3D11ShaderResourceView> m_ShaderResource;
		ComPtr<ID3D11Texture2D> m_TextureBuffer;

	private:
		// Special constructor that is used only by frame buffers;
		DX11Texture2D(uint32 width, uint32 height, const ComPtr<ID3D11ShaderResourceView>& SRV, TextureFormat format);
	};
}

#endif
