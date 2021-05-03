#pragma once

#include "Engine/Core/HLCore.h"
#include "Engine/Core/HLAllocator.h"
#include "Engine/Renderer/ImageFormat.h"

namespace highlo
{
	class Image : public IsSharedReference
	{
	public:

		virtual ~Image() {}

		virtual void Invalidate() = 0;
		virtual void Release() = 0;

		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;

		virtual ImageFormat GetFormat() const = 0;

		virtual Allocator GetBuffer() const = 0;
		virtual Allocator &GetBuffer() = 0;

		virtual uint64 GetHash() const = 0;
		virtual HLRendererID GetRendererID() const = 0;
		virtual HLRendererID GetSamplerRendererID() const = 0;
	};

	class Image2D : public Image
	{
	public:
	
		static Ref<Image2D> Create(ImageFormat format, uint32 width, uint32 height, Allocator buffer);
		static Ref<Image2D> Create(ImageFormat format, uint32 width, uint32 height, const void *data = nullptr);
	};
}