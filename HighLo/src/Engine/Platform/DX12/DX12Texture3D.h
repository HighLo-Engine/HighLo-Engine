// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2022-02-02) initial release
//

#pragma once

#include "Engine/Renderer/Texture3D.h"

#ifdef HIGHLO_API_DX12

namespace highlo
{
	class DX12Texture3D : public Texture3D
	{
	public:

		DX12Texture3D(const FileSystemPath &filePath, bool flipOnLoad = false);
		DX12Texture3D(TextureFormat format, uint32 width, uint32 height, const void *data);
		virtual ~DX12Texture3D();

		virtual uint32 GetWidth() const override { return m_Specification.Width; }
		virtual uint32 GetHeight() const override { return m_Specification.Height; }
		virtual Allocator GetData() override;

		virtual void Bind(uint32 slot) const override;
		virtual void Unbind(uint32 slot) const override;

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

	private:

		HLRendererID m_RendererID = 0;
		TextureSpecification m_Specification;

		bool m_Loaded = false;
		bool m_Locked = false;
	};
}

#endif // HIGHLO_API_DX12

